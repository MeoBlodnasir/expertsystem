
#include "Core.h"
#include "VariablesManager.h"

namespace ft
{
	VariablesManager::VariablesManager()
	{
	}

	VariablesManager::~VariablesManager()
	{
	}

	const Variable*	VariablesManager::CreateVariable(Variable::Id iId, bool bCreationState /*= false*/, bool bCreationLock /*= false*/)
	{
		if (m_oVariables.count(iId) == 0)
			m_oVariables[iId] = Variable(iId, bCreationState, bCreationLock);

		return &m_oVariables.at(iId);
	}

	const Variable*	VariablesManager::GetVariable(Variable::Id iId)
	{
		return CreateVariable(iId);
	}

	void			VariablesManager::SetVariableState(Variable::Id iId, bool bState)
	{
		CreateVariable(iId);
		m_oVariables[iId].m_bState = bState;
	}
	
	void			VariablesManager::SetVariableLock(Variable::Id iId, bool bLock)
	{
		CreateVariable(iId);
		m_oVariables[iId].m_bLock = bLock;
	}
}
