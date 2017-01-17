
#include "RuleStack.h"

#include "Variable.h"
#include "Operator.h"

#include <stack>

namespace ft
{
	RuleStack::RuleStack()
		: m_oElements()
	{
	}

	RuleStack::~RuleStack()
	{
	}

	void	RuleStack::AddElement(const IRuleElement* pElement)
	{
		m_oElements.push_front(pElement);
	}

	bool	RuleStack::EvaluateStack() const
	{
		std::stack<bool>	oResultStack;

		for (RuleDeque::const_iterator it = m_oElements.begin(), itEnd = m_oElements.end(); it != itEnd; ++it)
		{
			if ((*it)->GetType() == IRuleElement::E_VARIABLE)
			{
				oResultStack.push(reinterpret_cast<const Variable*>(*it)->GetState());
			}
			else if ((*it)->GetType() == IRuleElement::E_OPERATOR)
			{
				const uint32 iOperandCount = reinterpret_cast<const AOperator*>(*it)->iOperandCount;

				if (iOperandCount == 1)
				{
					oResultStack.top() = reinterpret_cast<const AUnaryOperator*>(*it)->Evaluate(oResultStack.top());
				}
				else if (iOperandCount == 2)
				{
					bool bTemp = oResultStack.top();
					oResultStack.pop();
					oResultStack.top() = reinterpret_cast<const ABinaryOperator*>(*it)->Evaluate(bTemp, oResultStack.top());
				}
				else
				{
					// erreur
				}
			}
			else
			{
				// erreur
			}
		}

		if (oResultStack.size() == 0)
		{
			// erreur
		}

		return oResultStack.top();
	}
}
