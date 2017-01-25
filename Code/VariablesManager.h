#pragma once

#include "ErrorCode.h"
#include "Variable.h"

#include <map>

namespace ft
{
	class VariablesManager
	{
	public:

		VariablesManager();
		~VariablesManager();

		const Variable*	CreateVariable(Variable::Id iId, bool  bCreationState = false, bool bCreationLock = false);
		const Variable*	GetVariable(Variable::Id iId) const;

		EErrorCode		SetVariableState(Variable::Id iId, bool bState);
		EErrorCode		SetVariableLock(Variable::Id iId, bool bLock);
		bool			GetLockState(Variable::Id iId);

	private:

		std::map<Variable::Id, Variable>	m_oVariables;

		VariablesManager(const VariablesManager&);
		VariablesManager& operator = (const VariablesManager&);
	};
}
