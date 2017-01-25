#pragma once

#include "ErrorCode.h"
#include "Token.h"
#include "RulesManager.h"

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

			VariablesManager*	m_pVariablesManager;
			RulesManager*		m_pRulesManager;
			OperatorsProvider*	m_pOperatorProvider;
			std::vector<char>	m_oPendingQueries;

			EErrorCode	ReadTokens(const std::vector<Token>& oTokens);
	};
}
