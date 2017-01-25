#pragma once

#include "ErrorCode.h"
#include "VariablesManager.h"
#include "Rule.h"

#include <map>

namespace ft
{
	class RulesManager
	{
	public:

		RulesManager();
		RulesManager(VariablesManager *oVariablesManager);
		~RulesManager();

		void	AddRule(Rule rule);
		EErrorCode	EvaluateRules();


	private:

		std::vector<Rule>	m_oRules;
		VariablesManager	*m_oVariablesManager;

		RulesManager(const RulesManager&);
		RulesManager& operator = (const RulesManager&);
	};
}
