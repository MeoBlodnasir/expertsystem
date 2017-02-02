
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
	
	void			VariablesManager::PrintVariables() const
	{
		FT_COUT << "VARIABLES MANAGER" << std::endl;

		if (m_oVariables.size() == 0)
			FT_COUT << "Aucune variable" << std::endl;

		for (const std::pair<Variable::Id, Variable>& itVar : m_oVariables)
			FT_COUT << itVar.first << " : " << itVar.second.GetState() << std::endl;
	}
}
