#pragma once

#include "RuleStack.h"

namespace ft
{
	class Rule
	{
	public:

		Rule();
		~Rule();

		// Evaluate

	private:

		RuleStack	m_oRuleStack;
	};
}
