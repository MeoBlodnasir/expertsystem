#pragma once

#include "ErrorCode.h"
#include "Variable.h"
#include "Token.h"
#include "Rule.h"
#include "RulesManager.h"

#include <vector>

namespace ft
{
	// fw
	class VariablesManager;
	class InferenceEngine;

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
		InferenceEngine*			m_pInferenceEngine;
		RulesManager*				m_pRulesManager;
		std::vector<Variable::Id>	m_oPendingQueries;

		EErrorCode	ReadTokens(const std::vector<Token>& oTokens);
	};
}
