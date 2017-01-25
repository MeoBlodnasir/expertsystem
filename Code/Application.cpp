
#include "Application.h"

#include "File.h"
#include "Lexer.h"
#include "VariablesManager.h"
#include "RulesManager.h"
#include "OperatorsProvider.h"
#include "Parser.h"

#include "Output.h" // tmp

namespace ft
{
	Application::Application()
		: m_pVariablesManager(nullptr)
		, m_pOperatorProvider(nullptr)
		, m_pRulesManager(nullptr)
	{
	}

	Application::~Application()
	{
	}

	EErrorCode	Application::Init(int ac, char **av)
	{
		(void)ac; (void)av;

		std::string			sFileContent;
		std::vector<Token>	oTokens;

		m_pVariablesManager = new VariablesManager();
		m_pRulesManager = new RulesManager(m_pVariablesManager);
		m_pOperatorProvider = new OperatorsProvider();

		FT_TEST_OK(File::GetContent(&sFileContent, "./Assets/test.txt"));
		FT_TEST_OK(Lexer::ReadInput(&oTokens, sFileContent.c_str()));
		FT_TEST_OK(ReadTokens(oTokens));

		return FT_OK;
	}

	EErrorCode	Application::Destroy()
	{
		FT_SAFE_DELETE(m_pVariablesManager);
		FT_SAFE_DELETE(m_pRulesManager);
		FT_SAFE_DELETE(m_pOperatorProvider);

		return FT_OK;
	}

	EErrorCode	Application::Run()
	{
		// V�rifie que c'est initialis�
		FT_ASSERT(m_pVariablesManager != nullptr);
		FT_ASSERT(m_pRulesManager != nullptr);
		FT_ASSERT(m_pOperatorProvider != nullptr);

		Rule	oRule;

		const Variable*	pA = nullptr;
		const Variable*	pB = nullptr;
		const Variable*	pC = nullptr;
		const Variable*	pD = nullptr;

		pA = m_pVariablesManager->CreateVariable('A', true, false);
		pB = m_pVariablesManager->CreateVariable('B', false, false);
		pC = m_pVariablesManager->CreateVariable('C', false, false);
		pD = m_pVariablesManager->CreateVariable('D', false, false);

		oRule.AddConditionElement(pA);
		oRule.AddConditionElement(pA);
		oRule.AddResultElement(pB);
		oRule.AddConditionElement(m_pOperatorProvider->And());
		oRule.Evaluate();	// true


		FT_COUT << pB->GetState() << std::endl;
		m_pRulesManager->AddRule(oRule);
		m_pRulesManager->EvaluateRules();
		FT_COUT << pB->GetState() << std::endl;


		oRule.AddConditionElement(pB);
		oRule.AddConditionElement(m_pOperatorProvider->And());
		oRule.Evaluate();	// false

		oRule.AddConditionElement(pA);
		oRule.AddConditionElement(m_pOperatorProvider->Or());
		oRule.Evaluate();	// true

		oRule.AddConditionElement(pA);
		oRule.AddConditionElement(m_pOperatorProvider->Xor());
		oRule.Evaluate();	// false

		oRule.AddConditionElement(pA);
		oRule.AddConditionElement(m_pOperatorProvider->Not());
		oRule.AddConditionElement(m_pOperatorProvider->Xor());
		oRule.Evaluate();	// false

		oRule.AddConditionElement(pB);
		oRule.AddConditionElement(m_pOperatorProvider->Not());
		oRule.AddConditionElement(m_pOperatorProvider->Or());
		oRule.Evaluate();	// true

		return FT_OK;
	}

	EErrorCode	Application::ReadTokens(const std::vector<Token>& oTokens)
	{
		enum ETokenReadingState
		{
			E_NONE = -1,
			E_RULE,
			E_FACTS,
			E_QUERIES
		};

		Parser				oParser;
		ETokenReadingState	eState = E_NONE;
		
		for (std::vector<Token>::const_iterator itToken = oTokens.begin(), itEnd = oTokens.end(); itToken != itEnd; ++itToken)
		{
			if (oParser.CheckToken(*itToken))
			{
				switch (eState)
				{
				case E_NONE:
					{
						switch (itToken->GetType())
						{
						case Token::E_EOF:
						case Token::E_EOL:
						case Token::E_COMMENT:					{ break; }

						case Token::E_SYM_START_FACTS:			{ eState = E_FACTS; break; }

						case Token::E_SYM_START_QUERIES:		{ eState = E_QUERIES; break; }

						case Token::E_SYM_OPEN_PAR:
						case Token::E_OP_LOGIC_NOT:
						case Token::E_VARIABLE:
							{
								// Cr�er une nouvelle r�gle
								eState = E_RULE;
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
						case Token::E_COMMENT:					{ eState = E_NONE; break; }

						case Token::E_SYM_OPEN_PAR:
						case Token::E_SYM_CLOSE_PAR:
						case Token::E_SYM_START_FACTS:
						case Token::E_SYM_START_QUERIES:
						case Token::E_OP_IMPLIES_IFANDONLYIF:
						case Token::E_OP_IMPLIES:
						case Token::E_OP_LOGIC_AND:
						case Token::E_OP_LOGIC_OR:
						case Token::E_OP_LOGIC_XOR:
						case Token::E_OP_LOGIC_NOT:
						case Token::E_VARIABLE:
							{
								// Ajouter l'�l�ment � la r�gle en cours
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
						case Token::E_COMMENT:					{ eState = E_NONE; break; }

						case Token::E_VARIABLE:
							{
								// Appliquer la valeur � la variable
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
						case Token::E_COMMENT:					{ eState = E_NONE; break; }

						case Token::E_VARIABLE:
							{
								// V�rifier ou cr�er dans le VariableManager avant ou pas?
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
}
