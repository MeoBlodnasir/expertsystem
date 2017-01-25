
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

	const Variable*	VariablesManager::GetVariable(Variable::Id iId) const
	{
		// Retour nullptr si la variable n'existe pas ou on la crée et on l'initialise à false?
		return m_oVariables.count(iId) == 1 ? &m_oVariables.at(iId) : nullptr;
	}

	EErrorCode		VariablesManager::SetVariableState(Variable::Id iId, bool bState)
	{
		// Retour fail si la variable n'existe pas ou on la crée? Voir à l'utilisation.
		FT_TEST_RETURN(m_oVariables.count(iId) == 1, FT_FAIL);

		m_oVariables[iId].m_bState = bState;
		
		return FT_OK;
	}
	
	EErrorCode		VariablesManager::SetVariableLock(Variable::Id iId, bool bLock)
	{
		// Retour fail si la variable n'existe pas ou on la crée? Voir à l'utilisation.
		FT_TEST_RETURN(m_oVariables.count(iId) == 1, FT_FAIL);

		m_oVariables[iId].m_bLock = bLock;
		
		return FT_OK;
	}

	bool		VariablesManager::GetLockState(Variable::Id iId)
	{
		return m_oVariables[iId].m_bLock;
	}
}
