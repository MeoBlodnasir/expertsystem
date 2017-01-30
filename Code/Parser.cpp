
#include "Parser.h"

#include "Atom.h"
#include "LogicOperator.h"

namespace ft
{
	Parser::Parser()
		: m_eRuleState(E_ANTECEDENT)
	{
	}

	Parser::~Parser()
	{
	}

	EErrorCode	Parser::ReadTokens(OutData* pData, const std::vector<Token>& oTokens)
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
					FT_TEST_OK(ReadFacts(pData, oTokens));
					break;
				}

			case Token::E_SYM_START_QUERIES:
				{
					FT_TEST_OK(ReadQueries(pData, oTokens));
					break;
				}

			case Token::E_SYM_OPEN_PAR:
			case Token::E_OP_LOGIC_NOT:
			case Token::E_VARIABLE:
				{
					FT_TEST_OK(ReadRule(pData, oTokens));
					break;
				}

			case Token::E_CMD_QUIT:
				{
					FT_TEST_OK(ReadCommand(pData, oTokens));
					break;
				}

			default:
				{
					FT_NOT_IMPLEMENTED("Token de début de ligne invalide");
					break;
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
		case Token::E_OP_LOGIC_NOT:	{ AddElementToRule(pRule, OperatorNOT());				break; }
		case Token::E_OP_LOGIC_AND:	{ AddElementToRule(pRule, OperatorAND());				break; }
		case Token::E_OP_LOGIC_OR:	{ AddElementToRule(pRule, OperatorOR());				break; }
		case Token::E_OP_LOGIC_XOR:	{ AddElementToRule(pRule, OperatorXOR());				break; }
		case Token::E_VARIABLE:		{ AddElementToRule(pRule, Atom(oToken.GetDesc()[0]));	break; }

		default:
			{
				FT_NOT_IMPLEMENTED("Erreur AddTokenToRule");
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

	EErrorCode	Parser::ReadRule(OutData* pData, const std::vector<Token>& oTokens)
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
		Rule						oRule;
		std::stack<const Token*>	oPendingTokens;
		int32						iWaitingStateFlags = E_WAITFOR_VARIABLE;
		int32						iParenthesisLevel = 0;
		
		oData.eDataType = OutData::E_RULE;
		m_eRuleState = E_ANTECEDENT;
		for (std::vector<Token>::const_iterator itToken = oTokens.begin(), itEnd = oTokens.end(); itToken != itEnd; ++itToken)
		{
			switch (itToken->GetType())
			{
			case Token::E_EOF:
			case Token::E_EOL:
			case Token::E_COMMENT:
				{
					if (!(iWaitingStateFlags & E_WAITFOR_EOL_EOF)
						|| iParenthesisLevel > 0
						|| m_eRuleState != E_CONSEQUENT)
					{
						FT_NOT_IMPLEMENTED("Erreur Rule Token");
						return FT_FAIL;
					}
					iWaitingStateFlags = E_NONE;

					UnstackPendingElements(&oPendingTokens, &oRule);
					break;
				}

			case Token::E_SYM_OPEN_PAR:
				{
					if (!(iWaitingStateFlags & E_WAITFOR_VARIABLE))
					{
						FT_NOT_IMPLEMENTED("Erreur Rule Token");
						return FT_FAIL;
					}
					++iParenthesisLevel;

					oPendingTokens.push(&(*itToken));
					break;
				}
			case Token::E_SYM_CLOSE_PAR:
				{
					if (oPendingTokens.empty()
						|| !(iWaitingStateFlags & E_WAITFOR_CLOSE_PAR)
						|| iParenthesisLevel == 0)
					{
						FT_NOT_IMPLEMENTED("Erreur Rule Token");
						return FT_FAIL;
					}
					--iParenthesisLevel;

					while (oPendingTokens.top()->GetType() != Token::E_SYM_OPEN_PAR)
					{
						AddTokenToRule(&oRule, *oPendingTokens.top());
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
						FT_NOT_IMPLEMENTED("Erreur Rule Token");
						return FT_FAIL;
					}
					iWaitingStateFlags = E_WAITFOR_VARIABLE;

					UnstackPendingElements(&oPendingTokens, &oRule);
					oRule.SetBidirectionnal(itToken->GetType() == Token::E_OP_IMPLIES_IFANDONLYIF);
					m_eRuleState = E_CONSEQUENT;
					break;
				}

			case Token::E_OP_LOGIC_NOT:
				{
					if (!(iWaitingStateFlags & E_WAITFOR_VARIABLE))
					{
						FT_NOT_IMPLEMENTED("Erreur Rule Token");
						return FT_FAIL;
					}
					iWaitingStateFlags = E_WAITFOR_VARIABLE;

					if (!oPendingTokens.empty())
					{
						if (oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_NOT)
						{
							AddTokenToRule(&oRule, *oPendingTokens.top());
							oPendingTokens.pop();
						}
					}
					oPendingTokens.push(&(*itToken));
					break;
				}

			case Token::E_OP_LOGIC_AND:
			case Token::E_OP_LOGIC_OR:
			case Token::E_OP_LOGIC_XOR:
				{
					if (!(iWaitingStateFlags & E_WAITFOR_OPERATOR))
					{
						FT_NOT_IMPLEMENTED("Erreur Rule Token");
						return FT_FAIL;
					}
					iWaitingStateFlags = E_WAITFOR_VARIABLE;

					if (!oPendingTokens.empty())
					{
						if (itToken->GetType() == Token::E_OP_LOGIC_AND)
						{
							while (!oPendingTokens.empty()
								&&	(oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_NOT
								|| oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_AND))
							{
								AddTokenToRule(&oRule, *oPendingTokens.top());
								oPendingTokens.pop();
							}
						}
						else if (itToken->GetType() == Token::E_OP_LOGIC_OR)
						{
							while (!oPendingTokens.empty()
								&&	(oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_NOT
								|| oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_AND
								|| oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_OR))
							{
								AddTokenToRule(&oRule, *oPendingTokens.top());
								oPendingTokens.pop();
							}
						}
						else if (itToken->GetType() == Token::E_OP_LOGIC_XOR)
						{
							while (!oPendingTokens.empty()
								&&	(oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_NOT
								|| oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_AND
								|| oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_OR
								|| oPendingTokens.top()->GetType() == Token::E_OP_LOGIC_XOR))
							{
								AddTokenToRule(&oRule, *oPendingTokens.top());
								oPendingTokens.pop();
							}
						}
					}
					oPendingTokens.push(&(*itToken));
					break;
				}

			case Token::E_VARIABLE:
				{
					if (!(iWaitingStateFlags & E_WAITFOR_VARIABLE))
					{
						FT_NOT_IMPLEMENTED("Erreur Rule Token");
						return FT_FAIL;
					}
					iWaitingStateFlags = E_WAITFOR_EOL_EOF | E_WAITFOR_OPERATOR | E_WAITFOR_IMPLY | E_WAITFOR_CLOSE_PAR;

					AddTokenToRule(&oRule, *itToken);
					oData.oAtoms.insert(itToken->GetDesc()[0]);
					break;
				}

			default:
				{
					FT_NOT_IMPLEMENTED("Erreur Rule Token");
					break;
				}
			}
		}

		oData.oRules.push_back(oRule);

		return FT_OK;
	}

	EErrorCode	Parser::ReadFacts(OutData* pData, const std::vector<Token>& oTokens)
	{
		FT_ASSERT(pData != nullptr);

		OutData& oData = *pData;
		oData.eDataType = OutData::E_FACTS;
		for (std::vector<Token>::const_iterator itToken = oTokens.begin(), itEnd = oTokens.end(); itToken != itEnd; ++itToken)
		{
			switch (itToken->GetType())
			{
			case Token::E_EOF:
			case Token::E_EOL:
			case Token::E_COMMENT:
			case Token::E_SYM_START_FACTS:	{ break; }

			case Token::E_VARIABLE:
				{
					oData.oAtoms.insert(itToken->GetDesc()[0]);
					break;
				}

			default:
				{
					FT_NOT_IMPLEMENTED("Erreur ReadFacts");
					break;
				}
			}
		}

		return FT_OK;
	}

	EErrorCode	Parser::ReadQueries(OutData* pData, const std::vector<Token>& oTokens)
	{
		FT_ASSERT(pData != nullptr);

		OutData& oData = *pData;
		oData.eDataType = OutData::E_QUERIES;
		for (std::vector<Token>::const_iterator itToken = oTokens.begin(), itEnd = oTokens.end(); itToken != itEnd; ++itToken)
		{
			switch (itToken->GetType())
			{
			case Token::E_EOF:
			case Token::E_EOL:
			case Token::E_COMMENT:
			case Token::E_SYM_START_QUERIES:	{ break; }

			case Token::E_VARIABLE:
				{
					oData.oAtoms.insert(itToken->GetDesc()[0]);
					break;
				}

			default:
				{
					FT_NOT_IMPLEMENTED("Erreur ReadQueries");
					break;
				}
			}
		}

		return FT_OK;
	}

	EErrorCode	Parser::ReadCommand(OutData* pData, const std::vector<Token>& oTokens)
	{
		FT_ASSERT(pData != nullptr);

		OutData& oData = *pData;
		oData.eDataType = OutData::E_COMMAND;
		oData.eCommandType = oTokens[0].GetType();

		return FT_OK;
	}
}
