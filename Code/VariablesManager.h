#pragma once

#include "ErrorCode.h"
#include "StrongPointer.h"
#include "Variable.h"

#include <map>

namespace ft
{
	class VariablesManager : public CountableSPtr
	{
	public:

		VariablesManager();
		~VariablesManager();

		const Variable&	CreateVariable(Variable::Id iId, bool bCreationState = false);

		const Variable*	GetVariable(Variable::Id iId) const;
		void			SetVariableState(Variable::Id iId, bool bState);

		template <class InputIterator>
		void			DeclareVariables(InputIterator itFirst, InputIterator itEnd, bool bCreationState = false)
		{
			for (InputIterator it = itFirst ; it != itEnd; ++it)
			{
				if (m_oVariables.count(*it) == 0)
					m_oVariables[*it] = Variable(*it, bCreationState);
			}
		}

		template <class InputIterator>
		void			SetVariables(InputIterator itFirst, InputIterator itEnd, bool bState)
		{
			for (InputIterator it = itFirst ; it != itEnd; ++it)
				SetVariableState(*it, bState);
		}

		void			DebugPrint() const;

	private:

		std::map<Variable::Id, Variable>	m_oVariables;

		VariablesManager(const VariablesManager&);
		VariablesManager& operator = (const VariablesManager&);
	};
}
