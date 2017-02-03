
#include "Parser.h"

#include "Atom.h"
#include "LogicOperator.h"
#include "Output.h"

namespace ft
{
	Parser::Parser()
		: m_eRuleState(E_ANTECEDENT)
	{
	}

	Parser::~Parser()
	{
	}

	EErrorCode	Parser::ReadTokens(OutData* pData, const std::vector<Token>& oTokens, const std::string& sLine)
	{
		FT_ASSERT(pData != nullptr);

		if (oTokens.size() > 0)
		{
			const Token& oToken = *oTokens.begin();

			switch (oToken.GetType())
			{
			case Token::E_EOF:
			case Token::E_EOL:
			case Token::E_COMMENT:
				{
					break;
				}

			case Token::E_SYM_START_FACTS:
				{
					if (ReadFacts(pData, oTokens, sLine) != FT_OK)
						return FT_FAIL;
					break;
				}

			case Token::E_SYM_START_QUERIES:
				{
					if (ReadQueries(pData, oTokens, sLine) != FT_OK)
						return FT_FAIL;
					break;
				}

			case Token::E_SYM_OPEN_PAR:
			case Token::E_OP_LOGIC_NOT:
			case Token::E_VARIABLE:
				{
					if (ReadRule(pData, oTokens, sLine) != FT_OK)
						return FT_FAIL;
					break;
				}

			case Token::E_CMD_VERBOSE:
			case Token::E_CMD_PRINT:
			case Token::E_CMD_RESET_VARIABLES:
			case Token::E_CMD_FLUSH:
			case Token::E_CMD_HELP:
			case Token::E_CMD_QUIT:
				{
					if (ReadCommand(pData, oTokens, sLine) != FT_OK)
						return FT_FAIL;
					break;
				}

			default:
				{
					FT_CERR << "Erreur ligne: " << sLine << std::endl;
					FT_CERR << "Token courant inconnu: " << oToken.GetDesc() << std::endl;
					return FT_FAIL;
				}
			}
		}

		return FT_OK;
	}

	void		Parser::AddElementToRule(Rule* pRule, const ILogicElement& oElement)
	{
		if (m_eRuleState == E_ANTECEDENT)
			pRule->AddAntecedentElement(oElement);
		else if (m_eRuleState == E_CONSEQUENT)
			pRule->AddConsequentElement(oElement);
	}

	void		Parser::AddTokenToRule(Rule* pRule, const Token& oToken)
	{
		FT_ASSERT(pRule != nullptr);

		switch (oToken.GetType())
		{
		case Token::E_OP_LOGIC_NOT:	{ AddElementToRule(pRule, OperatorNOT()); break; }
		case Token::E_OP_LOGIC_AND:	{ AddElementToRule(pRule, OperatorAND()); break; }
		case Token::E_OP_LOGIC_OR:	{ AddElementToRule(pRule, OperatorOR());  break; }
		case Token::E_OP_LOGIC_XOR:	{ AddElementToRule(pRule, OperatorXOR()); break; }
		case Token::E_VARIABLE:		{ AddElementToRule(pRule, Atom(oToken.GetDesc()[0])); break; }

		default:
			{
				FT_FAILED_ASSERTION("Attendu Token operateur ou variable");
				break;
			}
		}
	}

	void		Parser::UnstackPendingElements(std::stack<const Token*>* pPendingTokens, Rule* pRule)
	{
		FT_ASSERT(pPendingTokens != nullptr);
		FT_ASSERT(pRule != nullptr);

		while (!pPendingTokens->empty())
		{
			AddTokenToRule(pRule, *pPendingTokens->top());
			pPendingTokens->pop();
		}
	}

	EErrorCode	Parser::ReadRule(OutData* pData, const std::vector<Token>& oTokens, const std::string& sLine)
	{
		enum EWaitingStateFlag
		{
			E_NONE					= 0,
			E_WAITFOR_ENTRY			= (1 << 0),
			E_WAITFOR_SYMBOL		= (1 << 1),
			E_WAITFOR_VARIABLE		= (1 << 2),
			E_WAITFOR_OPERATOR		= (1 << 3),
			E_WAITFOR_IMPLY			= (1 << 4),
			E_WAITFOR_EOL_EOF		= (1 << 5),
			E_WAITFOR_CLOSE_PAR		= (1 << 6)
		};

		FT_ASSERT(pData != nullptr);

		OutData&					oData = *pData;
		std::stack<const Token*>	oPendingTokens;
		int32						iWaitingStateFlags = E_WAITFOR_VARIABLE;
		int32						iParenthesisLevel = 0;
		
		oData.eDataType = OutData::E_RULE;
		oData.oRule = Rule();
		m_eRuleState = E_ANTECEDENT;
		for (const Token& itToken : oTokens)
		{
			switch (itToken.GetType())
			{
			case Token::E_EOF:
			case Token::E_EOL:
			case Token::E_COMMENT:
				{
					if (!(iWaitingStateFlags & E_WAITFOR_EOL_EOF)
						|| iParenthesisLevel > 0
						|| m_eRuleState != E_CONSEQUENT)
					{
						PrintUnexpectedTokenError(sLine, "fin de ligne");
						return FT_FAIL;
					}
					iWaitingStateFlags = E_WAITFOR_EOL_EOF;

					UnstackPendingElements(&oPendingTokens, &oData.oRule);
					break;
				}

			case Token::E_SYM_OPEN_PAR:
				{
					if (!(iWaitingStateFlags & E_WAITFOR_VARIABLE))
					{
						PrintUnexpectedTokenError(sLine, itToken.GetDesc());
						return FT_FAIL;
					}
					++iParenthesisLevel;

					oPendingTokens.push(&itToken);
					break;
				}
			case Token::E_SYM_CLOSE_PAR:
				{
					if (oPendingTokens.empty()
						|| !(iWaitingStateFlags & E_WAITFOR_CLOSE_PAR)
						|| iParenthesisLevel == 0)
					{
						PrintUnexpectedTokenError(sLine, itToken.GetDesc());
						return FT_FAIL;
					}
					--iParenthesisLevel;

					while (oPendingTokens.top()->GetType() != Token::E_SYM_OPEN_PAR)
					{
						AddTokenToRule(&oData.oRule, *oPendingTokens.top());
						oPendingTokens.pop();
						FT_ASSERT(!oPendingTokens.empty());
					}
					oPendingTokens.pop();
					break;
				}

			case Token::E_OP_IMPLIES_IFANDONLYIF:
			case Token::E_OP_IMPLIES:
				{
					if (m_eRuleState != E_ANTECEDENT
						|| !(iWaitingStateFlags & E_WAITFOR_IMPLY)
						|| iParenthesisLevel > 0)
					{
						PrintUnexpectedTokenError(sLine, itToken.GetDesc());
						return FT_FAIL;
					}
					iWaitingStateFlags = E_WAITFOR_VARIABLE;

					UnstackPendingElements(&oPendingTokens, &oData.oRule);
					if (itToken.GetType() == Token::E_OP_IMPLIES_IFANDONLYIF)
						oData.oRule.SetBidirectionnal(true);

					m_eRuleState = E_CONSEQUENT;
					break;
				}

			case Token::E_OP_LOGIC_NOT:
				{
					if (!(iWaitingStateFlags & E_WAITFOR_VARIABLE))
					{
						PrintUnexpectedTokenError(sLine, itToken.GetDesc());
						return FT_FAIL;
					}
					iWaitingStateFlags = E_WAITFOR_VARIABLE;
					oPendingTokens.push(&itToken);
					break;
				}

			case Token::E_OP_LOGIC_AND:
			case Token::E_OP_LOGIC_OR:
			case Token::E_OP_LOGIC_XOR:
				{
					if (!(iWaitingStateFlags & E_WAITFOR_OPERATOR))
					{
						PrintUnexpectedTokenError(sLine, itToken.GetDesc());
						return FT_FAIL;
					}
					iWaitingStateFlags = E_WAITFOR_VARIABLE;

					if (!oPendingTokens.empty())
					{
						if (itToken.GetType() == Token::E_OP_LOGIC_AND)
						{
							while (!oPendingTokens.empty()
								&&	(oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_NOT))
							{
								AddTokenToRule(&oData.oRule, *oPendingTokens.top());
								oPendingTokens.pop();
							}
						}
						else if (itToken.GetType() == Token::E_OP_LOGIC_OR)
						{
							while (!oPendingTokens.empty()
								&&	(oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_NOT
								|| oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_AND))
							{
								AddTokenToRule(&oData.oRule, *oPendingTokens.top());
								oPendingTokens.pop();
							}
						}
						else if (itToken.GetType() == Token::E_OP_LOGIC_XOR)
						{
							while (!oPendingTokens.empty()
								&&	(oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_NOT
								|| oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_AND
								|| oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_OR))
							{
								AddTokenToRule(&oData.oRule, *oPendingTokens.top());
								oPendingTokens.pop();
							}
						}
					}
					oPendingTokens.push(&itToken);
					break;
				}

			case Token::E_VARIABLE:
				{
					if (!(iWaitingStateFlags & E_WAITFOR_VARIABLE))
					{
						PrintUnexpectedTokenError(sLine, itToken.GetDesc());
						return FT_FAIL;
					}
					iWaitingStateFlags = E_WAITFOR_EOL_EOF | E_WAITFOR_OPERATOR | E_WAITFOR_IMPLY | E_WAITFOR_CLOSE_PAR;

					AddTokenToRule(&oData.oRule, itToken);
					oData.oAtoms.insert(itToken.GetDesc()[0]);
					break;
				}

			default:
				{
					PrintUnexpectedTokenError(sLine, itToken.GetDesc());
					break;
				}
			}
		}

		oData.oRule.DeleteNotPairs();

		return FT_OK;
	}

	EErrorCode	Parser::ReadFacts(OutData* pData, const std::vector<Token>& oTokens, const std::string& sLine)
	{
		FT_ASSERT(pData != nullptr);

		OutData& oData = *pData;
		oData.eDataType = OutData::E_FACTS;
		for (const Token& itToken : oTokens)
		{
			switch (itToken.GetType())
			{
			case Token::E_EOF:
			case Token::E_EOL:
			case Token::E_COMMENT:
			case Token::E_SYM_START_FACTS:	{ break; }

			case Token::E_VARIABLE:
				{
					oData.oAtoms.insert(itToken.GetDesc()[0]);
					break;
				}

			default:
				{
					PrintUnexpectedTokenError(sLine, itToken.GetDesc());
					break;
				}
			}
		}

		return FT_OK;
	}

	EErrorCode	Parser::ReadQueries(OutData* pData, const std::vector<Token>& oTokens, const std::string& sLine)
	{
		FT_ASSERT(pData != nullptr);

		OutData& oData = *pData;
		oData.eDataType = OutData::E_QUERIES;
		for (const Token& itToken : oTokens)
		{
			switch (itToken.GetType())
			{
			case Token::E_EOF:
			case Token::E_EOL:
			case Token::E_COMMENT:
			case Token::E_SYM_START_QUERIES:	{ break; }

			case Token::E_VARIABLE:
				{
					oData.oAtoms.insert(itToken.GetDesc()[0]);
					break;
				}

			default:
				{
					PrintUnexpectedTokenError(sLine,itToken.GetDesc());
					break;
				}
			}
		}

		return FT_OK;
	}

	EErrorCode	Parser::ReadCommand(OutData* pData, const std::vector<Token>& oTokens, const std::string& /*sLine*/)
	{
		FT_ASSERT(pData != nullptr);

		OutData& oData = *pData;
		oData.eDataType = OutData::E_COMMAND;
		oData.eCommandType = oTokens[0].GetType();

		return FT_OK;
	}

	void		Parser::PrintUnexpectedTokenError(const std::string& sLine, const std::string& oTokenDesc) const
	{
		FT_CERR << "Erreur ligne: " << sLine << std::endl;
		FT_CERR << "Erreur de syntaxe: Token " << oTokenDesc << " inattendu" << std::endl;
	}
}
