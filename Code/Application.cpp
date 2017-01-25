
#include "Application.h"

#include "File.h"
#include "Lexer.h"
#include "VariablesManager.h"
#include "OperatorsProvider.h"
#include "Parser.h"

#include "Output.h" // tmp

namespace ft
{
	Application::Application()
		: m_pVariablesManager(nullptr)
		, m_pOperatorProvider(nullptr)
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
		m_pOperatorProvider = new OperatorsProvider();

		FT_TEST_OK(File::GetContent(&sFileContent, "./Assets/test.txt"));
		FT_TEST_OK(Lexer::ReadInput(&oTokens, sFileContent.c_str()));
		FT_TEST_OK(ReadTokens(oTokens));

		return FT_OK;
	}

	EErrorCode	Application::Destroy()
	{
		FT_SAFE_DELETE(m_pVariablesManager);
		FT_SAFE_DELETE(m_pOperatorProvider);

		return FT_OK;
	}

	EErrorCode	Application::Run()
	{
		// Vérifie que c'est initialisé
		FT_ASSERT(m_pVariablesManager != nullptr);
		FT_ASSERT(m_pOperatorProvider != nullptr);

		ft::Rule	oRule;

		const Variable*	pA = nullptr;
		const Variable*	pB = nullptr;
		const Variable*	pC = nullptr;
		const Variable*	pD = nullptr;

		pA = m_pVariablesManager->CreateVariable('A', Variable::E_TRUE);
		pB = m_pVariablesManager->CreateVariable('B', Variable::E_FALSE);
		pC = m_pVariablesManager->CreateVariable('C', Variable::E_FALSE);
		pD = m_pVariablesManager->CreateVariable('D', Variable::E_FALSE);

		oRule.AddConditionElement(pA);
		oRule.AddConditionElement(pA);
		oRule.AddConditionElement(m_pOperatorProvider->And());
		oRule.Evaluate();	// true

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
								// Créer une nouvelle règle
								eState = E_RULE;
								break;
							}

						default: { FT_NOT_IMPLEMENTED("Erreur Parsing"); break; }
						}
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
								// Ajouter l'élément à la règle en cours
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
								// Appliquer la valeur à la variable
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
								// Vérifier ou créer dans le VariableManager avant ou pas?
								m_oPendingQueries.push_back(itToken->GetDesc()[0]);
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
