#pragma once

#include "ErrorCode.h"
#include "RuleStack.h"
#include "Variable.h"

namespace ft
{
	class Rule
	{
	public:

		Rule();
		~Rule();

		void		AddConditionElement(const IRuleElement* pElement);
		void		AddResultElement(const IRuleElement* pElement);
		bool		Evaluate() const;
		Variable::Id	GetResultVariableId() const;

	private:

		ConditionRuleStack	m_oCondition;
		ResultRuleStack		m_oResult;
	};
}
