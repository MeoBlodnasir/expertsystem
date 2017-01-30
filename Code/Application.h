#pragma once

#include "ErrorCode.h"
#include "Rule.h"

#include <vector>

namespace ft
{
	// fw
	class VariablesManager;
	class RulesManager;
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

		VariablesManager*					m_pVariablesManager;
		RulesManager*						m_pRulesManager;
		InferenceEngine*					m_pInferenceEngine;
		std::vector<ILogicElement::AtomId>	m_oPendingQueries;
	};
}
