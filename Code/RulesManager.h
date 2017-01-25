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

		RulesManager(VariablesManager* pVariablesManager);
		~RulesManager();

		void		AddRule(const Rule& oRule);
		EErrorCode	EvaluateRules();

	private:

		std::vector<Rule>	m_oRules;
		VariablesManager*	m_pVariablesManager;

		RulesManager();
		RulesManager(const RulesManager&);
		RulesManager& operator = (const RulesManager&);
	};
}
