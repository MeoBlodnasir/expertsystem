
#include "RuleStack.h"

#include "Variable.h"
#include "Operator.h"

#include <stack>

namespace ft
{
	typedef std::vector<const IRuleElement*>::const_iterator	ElementsIt;

	//////////////////////////////
	// ARuleStack				//
	//////////////////////////////

	ARuleStack::ARuleStack()
	{
	}

	ARuleStack::~ARuleStack()
	{
	}

	void	ARuleStack::AddElement(const IRuleElement* pElement)
	{
		FT_ASSERT(pElement != nullptr);

		m_oElements.push_back(pElement);
	}

	//////////////////////////////
	// ConditionRuleStack		//
	//////////////////////////////

	ConditionRuleStack::ConditionRuleStack()
		: ARuleStack()
	{
	}

	ConditionRuleStack::~ConditionRuleStack()
	{
	}

	bool	ConditionRuleStack::Evaluate() const
	{
		FT_ASSERT(SelfAssert());

		std::stack<bool>	oResultStack;

		for (ElementsIt it = m_oElements.begin(), itEnd = m_oElements.end(); it != itEnd; ++it)
		{
			if ((*it)->GetType() == IRuleElement::E_VARIABLE)
			{
				bool	bState = reinterpret_cast<const Variable*>(*it)->GetState();
				oResultStack.push(bState == true);
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
					oResultStack.top() = reinterpret_cast<const ABinaryOperator*>(*it)->Evaluate(bTemp, oResultStack.top()); // vérifier pour l'ordre (1, 2) ou (2, 1)
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

		if (oResultStack.size() != 1)
		{
			FT_NOT_IMPLEMENTED("erreur");
		}

		return oResultStack.top();
	}

	bool	ConditionRuleStack::SelfAssert() const
	{
		return true;
	}

	//////////////////////////////
	// ResultRuleStack			//
	//////////////////////////////

	ResultRuleStack::ResultRuleStack()
		: ARuleStack()
	{
	}

	ResultRuleStack::~ResultRuleStack()
	{
	}
	
	Variable::Id	ResultRuleStack::GetTopVariableId() const
	{
		return (dynamic_cast<const Variable*>(m_oElements.front()))->GetId();
	}

	bool	ResultRuleStack::Evaluate() const
	{
		FT_ASSERT(SelfAssert());

		// Déterminer ce que veut dire "évaluer les résultats d'une règle"
		FT_TODO("ResultRuleStack::Evaluate()");

		return true;
	}

	bool	ResultRuleStack::SelfAssert() const
	{
		return true;
	}
}
