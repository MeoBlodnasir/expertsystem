
#include "Application.h"

#include "Core.h"
#include "File.h"
#include "Lexer.h"
#include "VariablesManager.h"
#include "InferenceEngine.h"
#include "LogicOperator.h"
#include "Proposition.h"
#include "Parser.h"

#include "Output.h" // tmp

namespace ft
{
	Application::Application()
		: m_pVariablesManager(nullptr)
		, m_pInferenceEngine(nullptr)
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
		m_pInferenceEngine = new InferenceEngine();

		FT_TEST_OK(File::GetContent(&sFileContent, "./Assets/Test_Parsing_01.txt"));
		FT_TEST_OK(Lexer::ReadInput(&oTokens, sFileContent.c_str()));
		FT_TEST_OK(ReadTokens(oTokens));

		return FT_OK;
	}

	EErrorCode	Application::Destroy()
	{
		FT_SAFE_DELETE(m_pVariablesManager);
		FT_SAFE_DELETE(m_pInferenceEngine);

		return FT_OK;
	}

	EErrorCode	Application::Run()
	{
		// Vérifie que c'est initialisé
		FT_ASSERT(m_pVariablesManager != nullptr);
		FT_ASSERT(m_pInferenceEngine != nullptr);


		// Création de Rules
		std::vector<Rule> oRules;

		FT_COUT << "A + B => C" << std::endl;
		oRules.push_back(Rule());
		oRules.back().AddAntecedentElement(Atom('A'));
		oRules.back().AddAntecedentElement(Atom('B'));
		oRules.back().AddAntecedentElement(OperatorAND());
		oRules.back().AddConsequentElement(Atom('C'));

		FT_COUT << "D | E => F" << std::endl;
		oRules.push_back(Rule());
		oRules.back().AddAntecedentElement(Atom('D'));
		oRules.back().AddAntecedentElement(Atom('E'));
		oRules.back().AddAntecedentElement(OperatorOR());
		oRules.back().AddConsequentElement(Atom('F'));

		FT_COUT << "C => G" << std::endl;
		oRules.push_back(Rule());
		oRules.back().AddAntecedentElement(Atom('C'));
		oRules.back().AddConsequentElement(Atom('G'));

		FT_COUT << "(!G + C) | F => H" << std::endl;
		oRules.push_back(Rule());
		oRules.back().AddAntecedentElement(Atom('G'));
		oRules.back().AddAntecedentElement(OperatorNOT());
		oRules.back().AddAntecedentElement(Atom('C'));
		oRules.back().AddAntecedentElement(OperatorAND());
		oRules.back().AddAntecedentElement(Atom('F'));
		oRules.back().AddAntecedentElement(OperatorOR());
		oRules.back().AddConsequentElement(Atom('H'));

		// Récupération des Atoms des Rules pour créer des Variables
		AtomIdSet	oAtomSet;
		for (std::vector<Rule>::const_iterator itRule = oRules.begin(), itEnd = oRules.end(); itRule != itEnd; ++itRule)
		{
			itRule->GetAntecedent().GetAtomsId(&oAtomSet);
			itRule->GetConsequent().GetAtomsId(&oAtomSet);
		}
		for (AtomIdSet::const_iterator itId = oAtomSet.begin(), itEnd = oAtomSet.end(); itId != itEnd; ++itId)
		{
			m_pVariablesManager->CreateVariable(*itId);
		}

		// Déclaration de faits initiaux
		// =AE
		m_pVariablesManager->CreateVariable('A', true);
		m_pVariablesManager->CreateVariable('E', true);
		m_pVariablesManager->DebugPrint();

		// Proposition d'évaluation de C
		bool bC = oRules[0].GetAntecedent().Evaluate(m_pVariablesManager);
		FT_COUT << "Evaluation de C : " << bC << std::endl;

		// Proposition d'évaluation de F
		bool bF = oRules[1].GetAntecedent().Evaluate(m_pVariablesManager);
		FT_COUT << "Evaluation de F : " << bF << std::endl;

		// Proposition d'évaluation de G
		Proposition	oPropG(oRules[2].GetAntecedent());
		oPropG.ReplaceAtom('C', oRules[0].GetAntecedent());
		bool bG = oPropG.Evaluate(m_pVariablesManager);
		FT_COUT << "Evaluation de G : " << bG << std::endl;

		// Proposition d'évaluation de H
		Proposition	oPropH(oRules[3].GetAntecedent());
		oPropH.ReplaceAtom('G', oRules[2].GetAntecedent());
		oPropH.ReplaceAtom('C', oRules[0].GetAntecedent());
		oPropH.ReplaceAtom('F', oRules[1].GetAntecedent());
		bool bH = oPropH.Evaluate(m_pVariablesManager);
		FT_COUT << "Evaluation de H : " << bH << std::endl;

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
