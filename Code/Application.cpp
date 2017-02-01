
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

#include "Output.h" // tmp

#include <stack>
#include <fstream>
#include <sstream>

namespace ft
{
	Application::Application()
		: m_ePendingCommand(E_CMD_NONE)
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
			//csFilePath = "./Assets/test08.txt"; (void)ac; (void)av;
			FT_COUT << "Fichier " << csFilePath << std::endl;
			oIFStream.open(csFilePath);
			if (oIFStream.rdstate() & std::ifstream::failbit)
			{
				FT_CERR << "Echec dans l'ouverture du fichier " << csFilePath << std::endl;
				continue;
			}
			oSStream << oIFStream.rdbuf();
			oIFStream.close();

			while (std::getline(oSStream, sLine))
			{
				if (ProcessInputLine(sLine) != FT_OK)
					continue;
			}

			EvaluatePendingQueries();
			
			// Reinitialiser les structures VariablesManager et RulesManager entre les fichiers
			m_xVariablesManager->Reset();
			m_xRulesManager->Reset();
			m_oPendingQueries.clear();
		}

		return FT_OK;
	}

	EErrorCode	Application::AskUserInput()
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		std::string	sLine;

		while (m_ePendingCommand != E_CMD_QUIT)
		{
			std::cout << "ENGIN T'ECOUTE MON ENFANT:" << std::endl;
			std::getline(std::cin, sLine);
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
		case Parser::OutData::E_RULE:
			{
				if (!m_xRulesManager->AddRule(oParsingData.oRule))
					return FT_FAIL;
				m_xVariablesManager->DeclareVariables(oParsingData.oAtoms.begin(), oParsingData.oAtoms.end());
				m_xRulesManager->CheckRules();
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
				case Token::E_CMD_VERBOSE:	{ m_ePendingCommand = E_CMD_VERBOSE;	break; }
				case Token::E_CMD_PRINT:	{ m_ePendingCommand = E_CMD_PRINT;		break; }
				case Token::E_CMD_FLUSH:	{ m_ePendingCommand = E_CMD_FLUSH;		break; }
				case Token::E_CMD_QUIT:		{ m_ePendingCommand = E_CMD_QUIT;		break; }

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
				m_ePendingCommand = E_CMD_NONE;
				break;
			}
		case E_CMD_PRINT:
			{
				PrintCurrentState();
				m_ePendingCommand = E_CMD_NONE;
				break;
			}
		case E_CMD_FLUSH:
			{
				m_xVariablesManager->Reset();
				m_xRulesManager->Reset();
				m_oPendingQueries.clear();
				m_ePendingCommand = E_CMD_NONE;
				break;
			}
		default:
			break;
		}
	}

	EErrorCode	Application::EvaluatePendingQueries()
	{
		FT_ASSERT(m_xVariablesManager != nullptr);
		FT_ASSERT(m_xRulesManager != nullptr);
		FT_ASSERT(m_xInferenceEngine != nullptr);

		if (m_oPendingQueries.size() > 0)
		{
			for (AtomIdSet::const_iterator itQuery = m_oPendingQueries.begin(), itEnd = m_oPendingQueries.end(); itQuery != itEnd; ++itQuery)
			{
				FT_COUT << "Evaluation de " << *itQuery << " : " << m_xInferenceEngine->ProcessQuery(*m_xVariablesManager, *m_xRulesManager, *itQuery) << std::endl;
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

		FT_COUT << "###################################" << std::endl;
		m_xVariablesManager->DebugPrint();
		m_xRulesManager->PrintRules();
		FT_COUT << std::endl;
		FT_COUT << "###################################" << std::endl;
	}
}

