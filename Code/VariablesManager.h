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

		const Variable*	CreateVariable(Variable::Id iId, bool bCreationState = false, bool bCreationLock = false);

		// Les accesseurs et mutateurs créent la variable requise si elle n'existe pas
		const Variable*	GetVariable(Variable::Id iId);

		void			SetVariableState(Variable::Id iId, bool bState);
		void			SetVariableLock(Variable::Id iId, bool bLock);

	private:

		std::map<Variable::Id, Variable>	m_oVariables;

		VariablesManager(const VariablesManager&);
		VariablesManager& operator = (const VariablesManager&);
	};
}
