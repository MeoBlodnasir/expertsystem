
#include "Application.h"

#include "File.h"
#include "Variable.h"
#include "Operator.h"
#include "Rule.h"
#include "Lexer.h"
#include "Parser.h"

namespace ft
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	EErrorCode	Application::Init(int ac, char **av)
	{
		(void)ac; (void)av;

		std::string				sFileContent;
		std::vector<Token>		oTokens;
		Parser::ParsingResult	oParsingResult;

		FT_TEST_OK(File::Read(&sFileContent, "./test.txt"));
		FT_TEST_OK(Lexer::ReadInput(&oTokens, sFileContent.c_str()));

		oParsingResult.pVariableStorage = &m_oVariableStorage;
		FT_TEST_OK(Parser::ReadTokens(&oParsingResult, oTokens));

		// Appliquer oParsingResult à m_oVariableStorage, m_oRules et m_oPendingQueries

		return FT_OK;
	}

	EErrorCode	Application::Destroy()
	{
		return FT_OK;
	}

	EErrorCode	Application::Run()
	{
		ft::Rule		oRule;

		ft::Variable	oT(true, 'T');
		ft::Variable	oF(false, 'F');

		ft::OperatorNOT	oNot;
		ft::OperatorAND	oAnd;
		ft::OperatorOR	oOr;
		ft::OperatorXOR	oXor;

		oRule.AddConditionElement(&oT);
		oRule.AddConditionElement(&oT);
		oRule.AddConditionElement(&oAnd);
		oRule.Evaluate();	// true

		oRule.AddConditionElement(&oF);
		oRule.AddConditionElement(&oAnd);
		oRule.Evaluate();	// false

		oRule.AddConditionElement(&oT);
		oRule.AddConditionElement(&oOr);
		oRule.Evaluate();	// true

		oRule.AddConditionElement(&oT);
		oRule.AddConditionElement(&oXor);
		oRule.Evaluate();	// false

		oRule.AddConditionElement(&oT);
		oRule.AddConditionElement(&oNot);
		oRule.AddConditionElement(&oXor);
		oRule.Evaluate();	// false

		oRule.AddConditionElement(&oF);
		oRule.AddConditionElement(&oNot);
		oRule.AddConditionElement(&oOr);
		oRule.Evaluate();	// true

		return FT_OK;
	}
}
