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

		const Variable*	CreateVariable(Variable::Id iId, Variable::EState bCreationState = Variable::E_UNDEF);
		const Variable*	GetVariable(Variable::Id iId) const;

		EErrorCode		SetVariableState(Variable::Id iId, Variable::EState bState);

	private:

		std::map<Variable::Id, Variable>	m_oVariables;

		VariablesManager(const VariablesManager&);
		VariablesManager& operator = (const VariablesManager&);
	};
}
