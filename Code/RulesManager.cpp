
#include "RulesManager.h"

#include "Core.h"
#include "VariablesManager.h"
#include "Output.h"

namespace ft
{
	RulesManager::RulesManager()
		: m_pVariablesManager(nullptr)
	{
	}

	RulesManager::RulesManager(VariablesManager* pVariablesManager)
	{
		m_pVariablesManager = pVariablesManager;
	}

	RulesManager::~RulesManager()
	{
	}

	void		RulesManager::AddRule(const Rule& oRule)
	{
		m_oRules.push_back(oRule);
	}

	EErrorCode	RulesManager::EvaluateRules()
	{
		for (std::vector<Rule>::const_iterator it = m_oRules.begin(), itEnd = m_oRules.end(); it != itEnd; it++)
		{
			if (it->Evaluate()) // Rule is verified
			{
				Variable::Id id = it->GetResultVariableId();
				if (!m_pVariablesManager->GetVariable(id)->IsLocked())
				{
					m_pVariablesManager->SetVariableState(id, true);
					m_pVariablesManager->SetVariableLock(id, true);
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
