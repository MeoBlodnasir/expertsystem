#pragma once

#include "ErrorCode.h"
#include "Variable.h"

#include <map>

namespace ft
{
	class VariableStorage
	{
	public:

		VariableStorage();
		~VariableStorage();

		const Variable*	CreateVariable(Variable::Id iId);
		const Variable*	GetVariable(Variable::Id iId) const;

		EErrorCode		SetVariableState(Variable::Id iId, bool bState);

	private:

		std::map<Variable::Id, Variable>	m_oVariables;

		VariableStorage(const VariableStorage&);
		VariableStorage& operator = (const VariableStorage&);
	};
}
