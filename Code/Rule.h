#pragma once

#include "ErrorCode.h"
#include "RuleStack.h"

namespace ft
{
	class Rule
	{
	public:

		Rule();
		~Rule();

		void		AddConditionElement(const IRuleElement* pElement);
		void		AddResultElement(const IRuleElement* pElement);
		EErrorCode	Evaluate() const;

	private:

		ConditionRuleStack	m_oCondition;
		ResultRuleStack		m_oResult;
	};
}
