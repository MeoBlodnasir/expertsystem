#pragma once

#include <deque> // queue, stack ?

namespace ft
{
	// fw
	class IRuleElement;

	class RuleStack
	{
	public:

		RuleStack();
		RuleStack(const RuleStack& oRuleStack);
		~RuleStack();

		// AddElement
		// EvaluateStack

	private:

		std::deque<IRuleElement*>	m_oElements;
	};
}
