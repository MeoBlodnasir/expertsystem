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
		~RuleStack();

		void	AddElement(const IRuleElement* pElement);
		bool	EvaluateStack() const;

	private:

		typedef std::deque<const IRuleElement*>	RuleDeque;
		RuleDeque	m_oElements;
	};
}
