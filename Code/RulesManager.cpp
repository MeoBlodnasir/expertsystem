
#include "Core.h"
#include "RulesManager.h"
#include "Output.h"
#include "Variable.h"
#include <iterator>

namespace ft
{
	RulesManager::RulesManager()
	{
	}

	RulesManager::RulesManager(VariablesManager *oVariablesManager)
	{
		m_oVariablesManager = oVariablesManager;
	}

	RulesManager::~RulesManager()
	{
	}

	void	RulesManager::AddRule(Rule rule)
	{
		m_oRules.push_back(rule);
	}

	EErrorCode	RulesManager::EvaluateRules()
	{
		for (std::vector<Rule>::const_iterator it = m_oRules.begin(), itEnd = m_oRules.end(); it != itEnd; it++)
		{
			if (it->Evaluate()) // Rule is verified
			{
				Variable::Id id = it->GetResultVariableId();
				if (!m_oVariablesManager->GetLockState(id))
				{
					m_oVariablesManager->SetVariableState(id, true);
					m_oVariablesManager->SetVariableLock(id, true);
				}
				FT_COUT << "rule is verified" << std::endl;
			}
			else
			{
				//	m_oRules.erase(it); // erasing invalid rule ? or just end program?
				FT_COUT << "rule not verified" << std::endl;
			}

		}

		return FT_OK;
	}


}
