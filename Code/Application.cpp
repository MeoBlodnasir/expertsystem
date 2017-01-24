
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

	EErrorCode Application::ReadTokens(const std::vector<Token>& oTokens)
	{
		Parser	oParser;
		
		for (std::vector<Token>::const_iterator it = oTokens.begin(), itEnd = oTokens.end(); it != itEnd; ++it)
		{
			if (oParser.CheckToken(*it))
			{
				FT_COUT << "ok" << std::endl; // tmp
			}
			else
			{
				FT_COUT << "nop" << std::endl; // tmp
			}
		}

		return FT_OK;
	}
}
