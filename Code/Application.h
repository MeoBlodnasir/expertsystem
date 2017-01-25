#pragma once

#include "ErrorCode.h"
#include "Variable.h"
#include "Token.h"
#include "Rule.h"

#include <vector>

namespace ft
{
	// fw
	class VariablesManager;
	class OperatorsProvider;

	class Application
	{
	public:

		Application();
		~Application();

		EErrorCode	Init(int ac, char **av);
		EErrorCode	Destroy();

		EErrorCode	Run();

	private:

		VariablesManager*			m_pVariablesManager;
		OperatorsProvider*			m_pOperatorProvider;
		std::vector<Rule>			m_oRules; // classe de gestionnaire de règles ? genre RulesManager
		std::vector<Variable::Id>	m_oPendingQueries;

		EErrorCode	ReadTokens(const std::vector<Token>& oTokens);
	};
}
