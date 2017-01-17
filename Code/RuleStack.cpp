
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
		FT_ASSERT(pElement != nullptr);

		m_oElements.push_back(pElement);
	}

	bool	RuleStack::Evaluate() const
	{
		std::stack<bool>	oResultStack;

		for (ElementsIt it = m_oElements.begin(), itEnd = m_oElements.end(); it != itEnd; ++it)
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
					oResultStack.top() = reinterpret_cast<const ABinaryOperator*>(*it)->Evaluate(bTemp, oResultStack.top()); // vérifier pour l'ordre (temp, top) ou (top, temp)
				}
				else
				{
					FT_NOT_IMPLEMENTED("erreur");
				}
			}
			else
			{
				FT_NOT_IMPLEMENTED("erreur");
			}
		}

		if (oResultStack.size() == 0)
		{
			FT_NOT_IMPLEMENTED("erreur");
		}

		return oResultStack.top();
	}
}
