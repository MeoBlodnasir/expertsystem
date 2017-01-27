
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
		Parser				oParser;
		Parser::ParsingData	oParsingData;

		m_pVariablesManager = new VariablesManager();
		m_pRulesManager = new RulesManager();
		m_pInferenceEngine = new InferenceEngine();

		FT_TEST_OK(File::GetContent(&sFileContent, "./Assets/Regles_Simples_03.txt"));
		FT_TEST_OK(Lexer::ReadInput(&oTokens, sFileContent.c_str()));

		oParsingData.pRules = &m_oRules;
		oParsingData.pFacts = m_pVariablesManager;
		oParsingData.pQueries = &m_oPendingQueries;
		FT_TEST_OK(oParser.ReadTokens(&oParsingData, oTokens));

		for (std::vector<Rule>::const_iterator itRule = m_oRules.begin(), itEnd = m_oRules.end(); itRule != itEnd; ++itRule)
		{
			FT_COUT << *itRule << std::endl;
		}

		m_pVariablesManager->DebugPrint();

		FT_COUT << '?';
		for (std::vector<ILogicElement::AtomId>::const_iterator itId = m_oPendingQueries.begin(), itEnd = m_oPendingQueries.end(); itId != itEnd; ++itId)
		{
			FT_COUT << *itId;
		}
		FT_COUT << std::endl;

		return FT_OK;
	}

	EErrorCode	Application::Destroy()
	{
		FT_SAFE_DELETE(m_pVariablesManager);
		FT_SAFE_DELETE(m_pRulesManager);
		FT_SAFE_DELETE(m_pInferenceEngine);

		return FT_OK;
	}

	EErrorCode	Application::Run()
	{
		// Vérifie que c'est initialisé
		FT_ASSERT(m_pVariablesManager != nullptr);
		FT_ASSERT(m_pInferenceEngine != nullptr);

		for (std::vector<ILogicElement::AtomId>::const_iterator itQuery = m_oPendingQueries.begin(), itEnd = m_oPendingQueries.end(); itQuery != itEnd; ++itQuery)
		{
			FT_COUT << "Evaluation de " << *itQuery << " : " << m_pInferenceEngine->ProcessQuery(*m_pVariablesManager, m_oRules, *itQuery) << std::endl;
		}

		Rule oRule;
		oRule.SetBidirectionnal(true);
		oRule.AddAntecedentElement(Atom('A'));
		oRule.AddConsequentElement(Atom('B'));
		oRule.AddConsequentElement(OperatorAND());
		oRule.AddConsequentElement(Atom('C'));
		oRule.AddConsequentElement(OperatorAND());
		oRule.AddConsequentElement(Atom('D'));
		m_pRulesManager->AddRule(oRule);
		m_pRulesManager->PrintRules();
		m_pRulesManager->DivideRules();
		m_pRulesManager->PrintRules();

		return FT_OK;
	}
}
