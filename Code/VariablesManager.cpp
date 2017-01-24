
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

	const Variable*	VariablesManager::CreateVariable(Variable::Id iId, Variable::EState bCreationState /*= Variable::E_UNDEF*/)
	{
		if (m_oVariables.count(iId) == 0)
			m_oVariables[iId] = Variable(iId, bCreationState);

		return &m_oVariables.at(iId);
	}

	const Variable*	VariablesManager::GetVariable(Variable::Id iId) const
	{
		// Retour nullptr si la variable n'existe pas ou on la crée et on l'initialise à false?
		return m_oVariables.count(iId) == 1 ? &m_oVariables.at(iId) : nullptr;
	}

	EErrorCode		VariablesManager::SetVariableState(Variable::Id iId, Variable::EState eState)
	{
		// Retour fail si la variable n'existe pas ou on la crée? Voir à l'utilisation.
		FT_TEST_RETURN(m_oVariables.count(iId) == 1, FT_FAIL);

		m_oVariables[iId].m_eState = eState;
		
		return FT_OK;
	}
}
