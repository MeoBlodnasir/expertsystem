
#include "Proposition.h"

#include "Types.h"
#include "Core.h"
#include "Atom.h"
#include "LogicOperator.h"

#include <stack>

namespace ft
{
	Proposition::Proposition()
		: IProposition()
	{
	}

	Proposition::Proposition(const Proposition& oProposition)
		: IProposition(oProposition)
	{
		for (const SPtr<ILogicElement>& xElem : oProposition.m_oElements)
		{
			m_oElements.push_back(xElem->Duplicate());
		}
	}

	Proposition::Proposition(const Atom& oAtom)
		: IProposition()
	{
		m_oElements.push_back(oAtom.Duplicate());
	}

	Proposition::~Proposition()
	{
	}

	std::string		Proposition::GetDesc() const 
	{
		std::string sDesc;
		for (const SPtr<ILogicElement>& xElem : m_oElements)
			sDesc += xElem->GetDesc();
		return sDesc;
	}

	ILogicElement*	Proposition::Duplicate() const
	{
		return new Proposition(*this);
	}

	bool	Proposition::Evaluate(const VariablesManager& pVariablesManager) const
	{
		FT_ASSERT(CheckValidity());

		std::stack<bool> oResultStack;

		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (xElem->GetType() == E_PROPOSITION || xElem->GetType() == E_ATOM)
			{
				bool bState = dynamic_cast<const IProposition*>(xElem.Get())->Evaluate(pVariablesManager);
				oResultStack.push(bState);
			}
			else if (xElem->GetType() == ILogicElement::E_OPERATOR)
			{
				const uint32 iOperandCount = dynamic_cast<const ALogicOperator*>(xElem.Get())->iOperandCount;

				if (iOperandCount == 1)
				{
					oResultStack.top() = dynamic_cast<const AUnaryOperator*>(xElem.Get())->Evaluate(oResultStack.top());
				}
				else if (iOperandCount == 2)
				{
					bool bTemp = oResultStack.top();
					oResultStack.pop();
					oResultStack.top() = dynamic_cast<const ABinaryOperator*>(xElem.Get())->Evaluate(bTemp, oResultStack.top()); // vérifier pour l'ordre (1, 2) ou (2, 1)
				}
				else
					FT_FAILED_ASSERTION("Operateur inconnu");
			}
			else
				FT_FAILED_ASSERTION("Type d'element inconnu");
		}

		FT_ASSERT(oResultStack.size() == 1);

		return oResultStack.top();
	}

	bool	Proposition::CheckValidity() const
	{
		// Simulation d'évaluation
		int32	iElementsCount = 0;

		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (xElem->GetType() == E_PROPOSITION || xElem->GetType() == E_ATOM)
			{
				if (!dynamic_cast<const IProposition*>(xElem.Get())->CheckValidity())
					return false;
				++iElementsCount;
			}
			else if (xElem->GetType() == ILogicElement::E_OPERATOR)
			{
				const uint32 iOperandCount = dynamic_cast<const ALogicOperator*>(xElem.Get())->iOperandCount;

				if (iOperandCount == 1 && iElementsCount > 0)
					continue;
				else if (iOperandCount == 2 && iElementsCount > 1)
					--iElementsCount;
				else
					return false;
			}
			else
				return false;
		}

		return iElementsCount == 1;
	}

	void	Proposition::GetAtomsId(AtomIdSet* pIdSet) const
	{
		FT_ASSERT(pIdSet != nullptr);

		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (xElem->GetType() == E_PROPOSITION)
			{
				dynamic_cast<const Proposition*>(xElem.Get())->GetAtomsId(pIdSet);
			}
			else if (xElem->GetType() == E_ATOM)
			{
				dynamic_cast<const Atom*>(xElem.Get())->GetAtomsId(pIdSet);
			}
		}
	}

	void	Proposition::AddElement(const ILogicElement& oElement)
	{
		m_oElements.push_back(oElement.Duplicate());
	}

	void	Proposition::ReplaceAtom(ILogicElement::AtomId iId, const IProposition& oNew)
	{
		for (SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (xElem->GetType() == E_PROPOSITION)
			{
				dynamic_cast<Proposition*>(xElem.Get())->ReplaceAtom(iId, oNew);
			}
			else if (xElem->GetType() == E_ATOM)
			{
				const Atom*	pAtom = dynamic_cast<const Atom*>(xElem.Get());
				if (pAtom->GetId() == iId)
				{
					xElem.Release();
					xElem = oNew.Duplicate();
				}
			}
		}
	}

	bool	Proposition::XorPresent() const
	{
		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (dynamic_cast<const OperatorXOR*>(xElem.Get()) != nullptr)
				return true;
		}
		return false;
	}

	bool	Proposition::AndPresent() const
	{
		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (dynamic_cast<const OperatorAND*>(xElem.Get()) != nullptr)
				return true;
		}
		return false;
	}

	bool	Proposition::OrPresent() const
	{
		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (dynamic_cast<const OperatorOR*>(xElem.Get()) != nullptr)
				return true;
		}
		return false;
	}

	bool	Proposition::NotPresent() const
	{
		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (dynamic_cast<const OperatorNOT*>(xElem.Get()) != nullptr)
				return true;
		}
		return false;
	}
}
