
#include "Application.h"

#include "Core.h"
#include "File.h"
#include "Lexer.h"
#include "VariablesManager.h"
#include "RulesManager.h"
#include "InferenceEngine.h"
#include "LogicOperator.h"
#include "Proposition.h"
#include "Parser.h"
#include "Output.h"

#include <fstream>
#include <sstream>

namespace ft
{
	Application::Application()
		: m_bQuitApplication(false)
		, m_ePendingCommand(E_CMD_NONE)
		, m_iOptions(E_OPT_NONE)
		, m_xVariablesManager(nullptr)
		, m_xRulesManager(nullptr)
		, m_xInferenceEngine(nullptr)
	{
	}

	Application::~Application()
	{
	}

	EErrorCode	Application::Init(const int32 ac, const char* const* av)
	{
		m_xVariablesManager	= new VariablesManager();
		m_xRulesManager		= new RulesManager();
		m_xInferenceEngine	= new InferenceEngine();

		FT_TEST_OK(ReadInputFiles(ac, av));

		return FT_OK;
	}

	EErrorCode	Application::Destroy()
	{
		m_xVariablesManager.Release();
		m_xRulesManager.Release();
		m_xInferenceEngine.Release();

		return FT_OK;
	}

	EErrorCode	Application::Run()
	{
		// Vérifie que c'est initialisé
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		FT_TEST_OK(AskUserInput());

		return FT_OK;
	}

	EErrorCode	Application::ReadInputFiles(const int32 ac, const char* const* av)
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		std::ifstream		oIFStream;
		std::stringstream	oSStream;
		std::string			sLine;
		const char*			csFilePath;

		for (int32 i = 1; i < ac; ++i)
		{
			csFilePath = av[i];
			FT_COUT << "Fichier " << csFilePath << std::endl;
			oIFStream.open(csFilePath);
			if (oIFStream.rdstate() & std::ifstream::failbit)
			{
				FT_CERR << "Echec dans l'ouverture du fichier " << csFilePath << std::endl;
				continue;
			}
			oSStream.clear();
			oSStream << oIFStream.rdbuf();
			oIFStream.close();

			while (std::getline(oSStream, sLine))
			{
				if (ProcessInputLine(sLine) != FT_OK)
					continue;
			}

			EvaluatePendingQueries();
			Flush();
		}

		return FT_OK;
	}

	EErrorCode	Application::AskUserInput()
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		std::string	sLine;

		std::cout << "\nEngin d'inference (\"help\" - \"quit\"):" << std::endl;

		while (!m_bQuitApplication)
		{
			std::cout << "> ";
			std::getline(std::cin, sLine);
			if (std::cin.rdstate() & std::ifstream::failbit)
				break;
			if (ProcessInputLine(sLine) != FT_OK)
				continue;

			EvaluatePendingQueries();
		}

		return FT_OK;
	}

	EErrorCode	Application::ProcessInputLine(const std::string& sLine)
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		Lexer::OutData	oLexingData;
		Parser			oParser;
		Parser::OutData	oParsingData;

		if (Lexer::ReadLine(&oLexingData, sLine) != FT_OK)
			return FT_FAIL;
		if (oParser.ReadTokens(&oParsingData, oLexingData.oTokens, sLine) != FT_OK)
			return FT_FAIL;

		switch (oParsingData.eDataType)
		{
		case Parser::OutData::E_NONE:
			{
				m_ePendingCommand = E_CMD_NONE;
				break;
			}

		case Parser::OutData::E_RULE:
			{
				if (!m_xRulesManager->AddRule(oParsingData.oRule))
					return FT_FAIL;
				m_xVariablesManager->DeclareVariables(oParsingData.oAtoms.begin(), oParsingData.oAtoms.end());
				break;
			}

		case Parser::OutData::E_FACTS:
			{
				m_xVariablesManager->SetVariables(oParsingData.oAtoms.begin(), oParsingData.oAtoms.end(), true);
				break;
			}

		case Parser::OutData::E_QUERIES:
			{
				m_xVariablesManager->DeclareVariables(oParsingData.oAtoms.begin(), oParsingData.oAtoms.end());
				m_oPendingQueries.insert(oParsingData.oAtoms.begin(), oParsingData.oAtoms.end());
				break;
			}

		case Parser::OutData::E_COMMAND:
			{
				switch (oParsingData.eCommandType)
				{
				case Token::E_CMD_VERBOSE:			{ m_ePendingCommand = E_CMD_VERBOSE;			break; }
				case Token::E_CMD_PRINT:			{ m_ePendingCommand = E_CMD_PRINT;				break; }
				case Token::E_CMD_RESET_VARIABLES:	{ m_ePendingCommand = E_CMD_RESET_VARIABLES;	break; }
				case Token::E_CMD_FLUSH:			{ m_ePendingCommand = E_CMD_FLUSH;				break; }
				case Token::E_CMD_HELP:				{ m_ePendingCommand = E_CMD_HELP;				break; }
				case Token::E_CMD_QUIT:				{ m_ePendingCommand = E_CMD_QUIT;				break; }

				default:
					{
						FT_CERR << "Commande inconnue: " << oLexingData.oTokens[0].GetDesc() << std::endl;
						return FT_FAIL;
					}
				}
				break;
			}

		default:
			break;
		}

		ProcessCommand();

		return FT_OK;
	}

	void	Application::ProcessCommand()
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		switch (m_ePendingCommand)
		{
		case E_CMD_VERBOSE:
			{
				m_iOptions ^= E_OPT_VERBOSE;
				m_xInferenceEngine->SetVerbose(m_iOptions & E_OPT_VERBOSE);
				break;
			}

		case E_CMD_PRINT:
			{
				PrintCurrentState();
				break;
			}

		case E_CMD_RESET_VARIABLES:
			{
				m_xVariablesManager->Reset();
				break;
			}

		case E_CMD_FLUSH:
			{
				Flush();
				break;
			}

		case E_CMD_HELP:
			{
				FT_COUT << "Commandes:" << std::endl;
				FT_COUT << "  verbose                   : Affiche les etapes d'evaluation" << std::endl;
				FT_COUT << "  print                     : Affiche les variables et regles courantes" << std::endl;
				FT_COUT << "  reset_variables           : Initialise toutes les variables a FAUX" << std::endl;
				FT_COUT << "  flush                     : Supprime les variables et les regles courantes" << std::endl;
				FT_COUT << "  quit                      : Termine le programme" << std::endl;

				FT_COUT << "Syntaxe:" << std::endl;
				FT_COUT << "  =[a]                      : Initialise les variables donnees a VRAI" << std::endl;
				FT_COUT << "  ?[a]                      : Demande au programme d'evaluer les variables donnees" << std::endl;
				FT_COUT << "  [a!+|^()] [[<]=>] [a!+()] : Ajoute une regle" << std::endl;
				break;
			}

		case E_CMD_QUIT:
			{
				m_bQuitApplication = true;
				break;
			}

		default:
			break;
		}

		m_ePendingCommand = E_CMD_NONE;
	}

	void	Application::Flush()
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);

		m_xVariablesManager->Flush();
		m_xRulesManager->Flush();
		m_oPendingQueries.clear();
	}

	EErrorCode	Application::EvaluatePendingQueries()
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		bool	bEvaluation;

		if (m_oPendingQueries.size() > 0)
		{
			for (ILogicElement::AtomId itQuery : m_oPendingQueries)
			{
				try
				{
					bEvaluation = m_xInferenceEngine->ProcessQuery(*m_xVariablesManager, *m_xRulesManager, itQuery);
					FT_COUT << "Evaluation de " << itQuery << " : " << (bEvaluation ? "VRAI" : "FAUX") << std::endl;
				}
				catch (InferenceEngine::ContradictionException&)
				{
					FT_COUT << "Contradiction dans les résultats au moment de l'évaluation" << std::endl;
				}
			}
			m_oPendingQueries.clear();
		}

		return FT_OK;
	}

	void	Application::PrintCurrentState() const
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		FT_COUT << "###################################\n" << std::endl;
		m_xVariablesManager->PrintVariables();
		FT_COUT << '\n';
		m_xRulesManager->PrintRules();
		FT_COUT << "\n###################################" << std::endl;
	}
}

