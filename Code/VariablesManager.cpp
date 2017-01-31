
#include "VariablesManager.h"

#include "Output.h"

namespace ft
{
	VariablesManager::VariablesManager()
	{
	}

	VariablesManager::~VariablesManager()
	{
	}

	void	VariablesManager::Reset()
	{
		m_oVariables.clear();
	}

	const Variable&	VariablesManager::CreateVariable(Variable::Id iId, bool bCreationState /*= false*/)
	{
		m_oVariables[iId] = Variable(iId, bCreationState);
		return m_oVariables.at(iId);
	}

	const Variable*	VariablesManager::GetVariable(Variable::Id iId) const
	{
		return m_oVariables.count(iId) > 0 ? &m_oVariables.at(iId) : nullptr;
	}

	void			VariablesManager::SetVariableState(Variable::Id iId, bool bState)
	{
		if (m_oVariables.count(iId) == 0)
			CreateVariable(iId);
		m_oVariables[iId].m_bState = bState;
	}
	
	void			VariablesManager::DebugPrint() const
	{
		FT_COUT << "VARIABLES MANAGER" << std::endl;
		FT_COUT << m_oVariables.size() << " variables" << std::endl;

		for (std::map<Variable::Id, Variable>::const_iterator itVar = m_oVariables.begin(), itEnd = m_oVariables.end();
			itVar != itEnd; ++itVar)
		{
			FT_COUT << itVar->first << " : " << itVar->second.GetState() << std::endl;
		}
	}
}
