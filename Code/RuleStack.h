#pragma once

#include <vector> // deque, queue, stack ?

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
		bool	Evaluate() const;

	private:

		std::vector<const IRuleElement*>	m_oElements;
		typedef std::vector<const IRuleElement*>::const_iterator	ElementsIt;
	};
}
