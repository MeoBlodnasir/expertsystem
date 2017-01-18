
#include "Core.h"
#include "VariableStorage.h"

namespace ft
{
	VariableStorage::VariableStorage()
		: m_oVariables()
	{
	}

	VariableStorage::~VariableStorage()
	{
	}

	const Variable*	VariableStorage::CreateVariable(Variable::Id iId)
	{
		if (m_oVariables.count(iId) == 0)
			m_oVariables[iId] = Variable(iId, false);

		return &m_oVariables.at(iId);
	}

	const Variable*	VariableStorage::GetVariable(Variable::Id iId) const
	{
		return m_oVariables.count(iId) == 1 ? &m_oVariables.at(iId) : nullptr;
	}

	EErrorCode		VariableStorage::SetVariableState(Variable::Id iId, bool bState)
	{
		FT_TEST_RETURN(m_oVariables.count(iId) == 1, FT_FAIL);

		m_oVariables[iId].SetState(bState);
		
		return FT_OK;
	}
}
