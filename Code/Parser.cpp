
#include "Parser.h"

#include "Core.h"
#include "Token.h"
#include "Rule.h"
#include "Atom.h"
#include "LogicOperator.h"
#include "VariablesManager.h"
#include "Output.h"

namespace ft
{
	Parser::Parser()
		: m_eReadingState(E_UNDEFINED)
		, m_iCheckTokenStateFlags(E_NONE)
		, m_iCheckTokenParenthesisLevel(0)
		, m_eRuleBuildingState(E_ANTECEDENT)
	{
		ResetParser();
	}

	Parser::~Parser()
	{
	}

	void		Parser::ResetParser()
	{
		m_eReadingState = E_UNDEFINED;
		ResetCheckTokenStates();
		ResetBuildRuleStates();
	}

	void		Parser::ResetCheckTokenStates()
	{
		m_iCheckTokenStateFlags = E_WAITFOR_EOL_EOF | E_WAITFOR_ENTRY | E_WAITFOR_VARIABLE;
		m_iCheckTokenParenthesisLevel = 0;
	}

	void		Parser::ResetBuildRuleStates()
	{
		m_oPendingElems = std::stack<const Token*>();
		m_eRuleBuildingState = E_ANTECEDENT;
	}

	EErrorCode	Parser::ReadTokens(ParsingData* pData, const std::vector<Token>& oTokens)
	{
		FT_ASSERT(pData != nullptr);
		FT_ASSERT(pData->pRules != nullptr);
		FT_ASSERT(pData->pFacts != nullptr);
		FT_ASSERT(pData->pQueries != nullptr);

		ParsingData&		oData = *pData;
		
		m_eReadingState = E_UNDEFINED;

		for (std::vector<Token>::const_iterator itToken = oTokens.begin(), itEnd = oTokens.end(); itToken != itEnd; ++itToken)
		{
			if (CheckToken(*itToken))
			{
				switch (m_eReadingState)
				{
				case E_UNDEFINED:
					{
						switch (itToken->GetType())
						{
						case Token::E_EOF:
						case Token::E_EOL:
						case Token::E_COMMENT: { break; }

						case Token::E_SYM_START_FACTS: { m_eReadingState = E_FACTS; break; }

						case Token::E_SYM_START_QUERIES: { m_eReadingState = E_QUERIES; break; }

						case Token::E_SYM_OPEN_PAR:
						case Token::E_OP_LOGIC_NOT:
						case Token::E_VARIABLE:
							{
								// Créer une nouvelle règle
								oData.pRules->push_back(Rule());
								ResetBuildRuleStates();
								BuildRule(&pData->pRules->back(), *itToken);
								if (itToken->GetType() == Token::E_VARIABLE)
								{
									if (!oData.pFacts->GetVariable(itToken->GetDesc()[0]))
										oData.pFacts->CreateVariable(itToken->GetDesc()[0]);
								}
								m_eReadingState = E_RULE;
								break;
							}

						default: { FT_NOT_IMPLEMENTED("Erreur Parsing"); break; }
						}
						break;
					}

				case E_RULE:
					{
						switch (itToken->GetType())
						{
						case Token::E_EOF:
						case Token::E_EOL:
						case Token::E_COMMENT:
							{
								// Dépile les éléments encore empilés
								BuildRule(&oData.pRules->back(), *itToken);
								m_eReadingState = E_UNDEFINED;
								break;
							}

						case Token::E_SYM_OPEN_PAR:
						case Token::E_SYM_CLOSE_PAR:
						case Token::E_OP_IMPLIES_IFANDONLYIF:
						case Token::E_OP_IMPLIES:
						case Token::E_OP_LOGIC_AND:
						case Token::E_OP_LOGIC_OR:
						case Token::E_OP_LOGIC_XOR:
						case Token::E_OP_LOGIC_NOT:
						case Token::E_VARIABLE:
							{
								// Ajouter l'élément à la règle en cours
								BuildRule(&oData.pRules->back(), *itToken);
								if (itToken->GetType() == Token::E_VARIABLE)
								{
									if (!oData.pFacts->GetVariable(itToken->GetDesc()[0]))
										oData.pFacts->CreateVariable(itToken->GetDesc()[0]);
								}
								break;
							}

						default: { FT_NOT_IMPLEMENTED("Erreur Parsing"); break; }
						}
						break;
					}

				case E_FACTS:
					{
						switch (itToken->GetType())
						{
						case Token::E_EOF:
						case Token::E_EOL:
						case Token::E_COMMENT: { m_eReadingState = E_UNDEFINED; break; }

						case Token::E_VARIABLE:
							{
								oData.pFacts->SetVariableState(itToken->GetDesc()[0], true);
								break;
							}

						default: { FT_NOT_IMPLEMENTED("Erreur Parsing"); break; }
						}
						break;
					}

				case E_QUERIES:
					{
						switch (itToken->GetType())
						{
						case Token::E_EOF:
						case Token::E_EOL:
						case Token::E_COMMENT: { m_eReadingState = E_UNDEFINED; break; }

						case Token::E_VARIABLE:
							{
								if (!oData.pFacts->GetVariable(itToken->GetDesc()[0]))
									oData.pFacts->CreateVariable(itToken->GetDesc()[0]);
								oData.pQueries->push_back(itToken->GetDesc()[0]);
								break;
							}

						default: { FT_NOT_IMPLEMENTED("Erreur Parsing"); break; }
						}
						break;
					}

				default: { FT_NOT_IMPLEMENTED("Erreur Parsing"); break; }
				}
			}
			else
			{
				FT_ASSERT(false);
				FT_COUT << "TOKEN NOP" << std::endl; // tmp
			}
		}

		return FT_OK;
	}

	bool		Parser::CheckToken(const Token& oToken)
	{
		// Pseudo State Machine pour vérifier si le Token envoyé est un Token attendu
		switch (oToken.GetType())
		{
		case Token::E_EOF:
			{
				if (!(m_iCheckTokenStateFlags & E_WAITFOR_EOL_EOF) || m_iCheckTokenParenthesisLevel > 0)
					return false;
				m_iCheckTokenStateFlags = E_NONE;
				break;
			}

		case Token::E_EOL:
			{
				if (!(m_iCheckTokenStateFlags & E_WAITFOR_EOL_EOF) || m_iCheckTokenParenthesisLevel > 0)
					return false;
				m_iCheckTokenStateFlags = E_WAITFOR_EOL_EOF | E_WAITFOR_ENTRY | E_WAITFOR_VARIABLE;
				break;
			}

		case Token::E_SYM_OPEN_PAR:
			{
				if (!(m_eReadingState == E_UNDEFINED || m_eReadingState == E_RULE)
					|| m_iCheckTokenStateFlags & E_DONTWAIT_OPEN_PAR)
					return false;
				++m_iCheckTokenParenthesisLevel;
				break;
			}

		case Token::E_SYM_CLOSE_PAR:
			{
				if (m_eReadingState != E_RULE || (m_iCheckTokenStateFlags & E_DONTWAIT_CLOSE_PAR) || m_iCheckTokenParenthesisLevel == 0)
					return false;
				--m_iCheckTokenParenthesisLevel;
				break;
			}

		case Token::E_SYM_START_FACTS:
		case Token::E_SYM_START_QUERIES:
			{
				if (m_eReadingState != E_UNDEFINED || !(m_iCheckTokenStateFlags & E_WAITFOR_ENTRY))
					return false;
				m_iCheckTokenStateFlags = E_WAITFOR_EOL_EOF | E_WAITFOR_VARIABLE | E_WAITONLY_VARIABLE | E_DONTWAIT_OPEN_PAR;
				break;
			}

		case Token::E_OP_IMPLIES:
		case Token::E_OP_IMPLIES_IFANDONLYIF:
			{
				if (m_eReadingState != E_RULE || !(m_iCheckTokenStateFlags & E_WAITFOR_IMPLY) || m_iCheckTokenParenthesisLevel > 0)
					return false;
				m_iCheckTokenStateFlags = E_WAITFOR_VARIABLE | E_DONTWAIT_CLOSE_PAR;
				break;
			}

		case Token::E_OP_LOGIC_NOT:
			{
				if (!(m_eReadingState == E_UNDEFINED || m_eReadingState == E_RULE)
					|| !(m_iCheckTokenStateFlags & E_WAITFOR_VARIABLE) || (m_iCheckTokenStateFlags & E_WAITONLY_VARIABLE))
					return false;
				m_iCheckTokenStateFlags = E_WAITFOR_VARIABLE | E_DONTWAIT_CLOSE_PAR;
				break;
			}

		case Token::E_OP_LOGIC_AND:
		case Token::E_OP_LOGIC_OR:
		case Token::E_OP_LOGIC_XOR:
			{
				if (m_eReadingState != E_RULE || !(m_iCheckTokenStateFlags & E_WAITFOR_OPERATOR))
					return false;
				m_iCheckTokenStateFlags = E_WAITFOR_VARIABLE | E_DONTWAIT_CLOSE_PAR;
				break;
			}

		case Token::E_VARIABLE:
			{
				if (!(m_iCheckTokenStateFlags & E_WAITFOR_VARIABLE))
					return false;
				if (m_eReadingState == E_QUERIES || m_eReadingState == E_FACTS)
					m_iCheckTokenStateFlags = E_WAITFOR_EOL_EOF | E_WAITFOR_VARIABLE;
				else
					m_iCheckTokenStateFlags = E_WAITFOR_EOL_EOF | E_WAITFOR_OPERATOR | E_WAITFOR_IMPLY;
				break;
			}

		case Token::E_COMMENT:
			{
				m_iCheckTokenStateFlags = E_WAITFOR_EOL_EOF;
				break;
			}

		default:
			return false;
		}

		return true;
	}

	void		Parser::UnstackPendingElements(Rule* pRule)
	{
		while (!m_oPendingElems.empty())
		{
			ForceAddElementToRuleFromToken(pRule, *m_oPendingElems.top());
			m_oPendingElems.pop();
		}
	}

	void		Parser::AddElementToRule(Rule* pRule, const ILogicElement& oElement)
	{
		if (m_eRuleBuildingState == E_ANTECEDENT)
			pRule->AddAntecedentElement(oElement);
		else if (m_eRuleBuildingState == E_CONSEQUENT)
			pRule->AddConsequentElement(oElement);
	}

	void		Parser::ForceAddElementToRuleFromToken(Rule* pRule, const Token& oToken)
	{
		switch (oToken.GetType())
		{
		case Token::E_OP_LOGIC_AND:	{ AddElementToRule(pRule, OperatorAND());				break; }
		case Token::E_OP_LOGIC_OR:	{ AddElementToRule(pRule, OperatorOR());				break; }
		case Token::E_OP_LOGIC_XOR:	{ AddElementToRule(pRule, OperatorXOR());				break; }
		case Token::E_OP_LOGIC_NOT:	{ AddElementToRule(pRule, OperatorNOT());				break; }
		case Token::E_VARIABLE:		{ AddElementToRule(pRule, Atom(oToken.GetDesc()[0]));	break; }

		default: { FT_NOT_IMPLEMENTED("Erreur ForceAddElementToRuleFromToken"); break; }
		}
	}

	void		Parser::BuildRule(Rule* pRule, const Token& oToken)
	{
		switch (oToken.GetType())
		{
		case Token::E_EOF:
		case Token::E_EOL:
		case Token::E_COMMENT:
			{
				FT_ASSERT(m_eRuleBuildingState == E_CONSEQUENT)
				// Dépile les éléments encore empilés
				UnstackPendingElements(pRule);
				break;
			}

		case Token::E_OP_IMPLIES_IFANDONLYIF:
		case Token::E_OP_IMPLIES:
			{
				FT_ASSERT(m_eRuleBuildingState == E_ANTECEDENT);
				// Dépile les éléments encore empilés
				UnstackPendingElements(pRule);
				pRule->SetBidirectionnal(oToken.GetType() == Token::E_OP_IMPLIES_IFANDONLYIF);
				m_eRuleBuildingState = E_CONSEQUENT;
				break;
			}

		case Token::E_SYM_OPEN_PAR:
		case Token::E_SYM_CLOSE_PAR:
			{
				if (oToken.GetType() == Token::E_SYM_OPEN_PAR)
				{
					m_oPendingElems.push(&oToken);
				}
				else
				{
					FT_ASSERT(!m_oPendingElems.empty());
					while (m_oPendingElems.top()->GetType() != Token::E_SYM_OPEN_PAR)
					{
						ForceAddElementToRuleFromToken(pRule, *m_oPendingElems.top());
						m_oPendingElems.pop();
						FT_ASSERT(!m_oPendingElems.empty());
					}
					m_oPendingElems.pop();
				}
				break;
			}

		case Token::E_OP_LOGIC_NOT:
		case Token::E_OP_LOGIC_AND:
		case Token::E_OP_LOGIC_OR:
		case Token::E_OP_LOGIC_XOR:
			{
				if (!m_oPendingElems.empty())
				{
					if (oToken.GetType() == Token::E_OP_LOGIC_NOT)
					{
						if (m_oPendingElems.top()->GetType() == Token::E_OP_LOGIC_NOT)
						{
							ForceAddElementToRuleFromToken(pRule, *m_oPendingElems.top());
							m_oPendingElems.pop();
						}
					}
					else if (oToken.GetType() == Token::E_OP_LOGIC_AND)
					{
						while (!m_oPendingElems.empty()
							&&	(m_oPendingElems.top()->GetType() == Token::E_OP_LOGIC_NOT
							  || m_oPendingElems.top()->GetType() == Token::E_OP_LOGIC_AND))
						{
							ForceAddElementToRuleFromToken(pRule, *m_oPendingElems.top());
							m_oPendingElems.pop();
						}
					}
					else if (oToken.GetType() == Token::E_OP_LOGIC_OR)
					{
						while (!m_oPendingElems.empty()
							&&	(m_oPendingElems.top()->GetType() == Token::E_OP_LOGIC_NOT
							  || m_oPendingElems.top()->GetType() == Token::E_OP_LOGIC_AND
							  || m_oPendingElems.top()->GetType() == Token::E_OP_LOGIC_OR))
						{
							ForceAddElementToRuleFromToken(pRule, *m_oPendingElems.top());
							m_oPendingElems.pop();
						}
					}
					else if (oToken.GetType() == Token::E_OP_LOGIC_XOR)
					{
						while (!m_oPendingElems.empty()
							&&	(m_oPendingElems.top()->GetType() == Token::E_OP_LOGIC_NOT
							  || m_oPendingElems.top()->GetType() == Token::E_OP_LOGIC_AND
							  || m_oPendingElems.top()->GetType() == Token::E_OP_LOGIC_OR
							  || m_oPendingElems.top()->GetType() == Token::E_OP_LOGIC_XOR))
						{
							ForceAddElementToRuleFromToken(pRule, *m_oPendingElems.top());
							m_oPendingElems.pop();
						}
					}
				}
				m_oPendingElems.push(&oToken);
				break;
			}

		case Token::E_VARIABLE:
			{
				ForceAddElementToRuleFromToken(pRule, oToken);
				break;
			}

		default: { FT_NOT_IMPLEMENTED("Erreur Parsing"); break; }
		}
	}

}
