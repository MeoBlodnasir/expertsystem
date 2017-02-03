
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

	bool	Proposition::operator == (const ILogicElement& oElem) const 
	{
		const Proposition* pProposition = dynamic_cast<const Proposition*>(&oElem);
		if (pProposition != nullptr)
		{
			bool bEquivalent = m_oElements.size() == pProposition->m_oElements.size();

			for (uint32 i = 0, iCount = m_oElements.size(); bEquivalent && i < iCount; ++i)
				bEquivalent &= *m_oElements[i] == *pProposition->m_oElements[i];

			return bEquivalent;
		}
		return false;
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
				{
					FT_FAILED_ASSERTION("Operateur inconnu");
				}
			}
			else
			{
				FT_FAILED_ASSERTION("Type d'element inconnu");
			}
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

	void	Proposition::DeleteNotPairs()
	{
		for (std::vector< SPtr<ILogicElement> >::const_iterator itElem = m_oElements.begin();
			itElem != m_oElements.end(); )
		{
			if (dynamic_cast<OperatorNOT*>((*itElem).Get()) != nullptr
				&& itElem + 1 != m_oElements.end()
				&& dynamic_cast<OperatorNOT*>((*(itElem + 1)).Get()) != nullptr)
			{
				// A faire dans cet ordre impérativement!
				// "all iterators, pointers and references to elements before position (or first)
				// are guaranteed to keep referring to the same elements
				// they were referring to before the call."
				m_oElements.erase(itElem + 1);
				itElem = m_oElements.erase(itElem);
				if (itElem != m_oElements.begin())
					--itElem;
			}
			else
				++itElem;
		}
	}

	bool	Proposition::IsXorPresent() const
	{
		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (dynamic_cast<const OperatorXOR*>(xElem.Get()) != nullptr)
				return true;
		}
		return false;
	}

	bool	Proposition::IsAndPresent() const
	{
		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (dynamic_cast<const OperatorAND*>(xElem.Get()) != nullptr)
				return true;
		}
		return false;
	}

	bool	Proposition::IsOrPresent() const
	{
		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (dynamic_cast<const OperatorOR*>(xElem.Get()) != nullptr)
				return true;
		}
		return false;
	}

	bool	Proposition::IsNotPresent() const
	{
		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (dynamic_cast<const OperatorNOT*>(xElem.Get()) != nullptr)
				return true;
		}
		return false;
	}

	bool	Proposition::IsConsequentAcceptedNot() const
	{
		// Vrai si l'opérateur Not s'applique directement à un Atom, ou si aucun opérateur Not n'est trouvé.
		// exemple: (A!) ou (A!B!+) ((!A) ou (!A + !B) en postfixe)
		// Sinon le cas n'est pas géré
		// exemple: (AB+!) ((!(A + B)) en postfixe)

		bool						bRet = true;
		const SPtr<ILogicElement>*	pPrecedent = nullptr;

		for (const SPtr<ILogicElement>& xElem : m_oElements)
		{
			if (dynamic_cast<const OperatorNOT*>(xElem.Get()) != nullptr)
				bRet &= (*pPrecedent)->GetType() == E_ATOM;
			pPrecedent = &xElem;
		}

		return bRet;
	}
}
