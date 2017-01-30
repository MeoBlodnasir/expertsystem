
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
		for (std::vector< SPtr<ILogicElement> >::const_iterator itElem = oProposition.m_oElements.begin(), itEnd = oProposition.m_oElements.end();
			itElem != itEnd; ++itElem)
		{
			m_oElements.push_back((*itElem)->Duplicate());
		}
	}

	Proposition::~Proposition()
	{
	}

	ILogicElement*	Proposition::Duplicate() const
	{
		return new Proposition(*this);
	}

	bool	Proposition::Evaluate(const VariablesManager& pVariablesManager) const
	{
		FT_ASSERT(CheckValidity());

		std::stack<bool> oResultStack;

		for (std::vector< SPtr<ILogicElement> >::const_iterator itElem = m_oElements.begin(), itEnd = m_oElements.end(); itElem != itEnd; ++itElem)
		{
			if ((*itElem)->GetType() == E_PROPOSITION || (*itElem)->GetType() == E_ATOM)
			{
				bool bState = dynamic_cast<const IProposition*>(itElem->Get())->Evaluate(pVariablesManager);
				oResultStack.push(bState);
			}
			else if ((*itElem)->GetType() == ILogicElement::E_OPERATOR)
			{
				const uint32 iOperandCount = dynamic_cast<const ALogicOperator*>(itElem->Get())->iOperandCount;

				if (iOperandCount == 1)
				{
					oResultStack.top() = dynamic_cast<const AUnaryOperator*>(itElem->Get())->Evaluate(oResultStack.top());
				}
				else if (iOperandCount == 2)
				{
					bool bTemp = oResultStack.top();
					oResultStack.pop();
					oResultStack.top() = dynamic_cast<const ABinaryOperator*>(itElem->Get())->Evaluate(bTemp, oResultStack.top()); // vérifier pour l'ordre (1, 2) ou (2, 1)
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

		FT_ASSERT(oResultStack.size() == 1);

		return oResultStack.top();
	}

	bool	Proposition::CheckValidity() const
	{
		std::stack<bool> oResultStack;

		for (std::vector< SPtr<ILogicElement> >::const_iterator itElem = m_oElements.begin(), itEnd = m_oElements.end(); itElem != itEnd; ++itElem)
		{
			if ((*itElem)->GetType() == E_PROPOSITION || (*itElem)->GetType() == E_ATOM)
			{
				if (!dynamic_cast<const IProposition*>(itElem->Get())->CheckValidity())
					return false;
				oResultStack.push(true);
			}
			else if ((*itElem)->GetType() == ILogicElement::E_OPERATOR)
			{
				const uint32 iOperandCount = dynamic_cast<const ALogicOperator*>(itElem->Get())->iOperandCount;

				if (iOperandCount == 1 && oResultStack.size() > 0)
					continue;
				else if (iOperandCount == 2 && oResultStack.size() > 1)
					oResultStack.pop();
				else
					return false;
			}
			else
				return false;
		}

		return oResultStack.size() == 1;
	}

	void	Proposition::GetAtomsId(AtomIdSet* pIdSet) const
	{
		FT_ASSERT(pIdSet != nullptr);

		for (std::vector< SPtr<ILogicElement> >::const_iterator itElem = m_oElements.begin(), itEnd = m_oElements.end(); itElem != itEnd; ++itElem)
		{
			if ((*itElem)->GetType() == E_PROPOSITION)
			{
				dynamic_cast<const Proposition*>(itElem->Get())->GetAtomsId(pIdSet);
			}
			else if ((*itElem)->GetType() == E_ATOM)
			{
				dynamic_cast<const Atom*>(itElem->Get())->GetAtomsId(pIdSet);
			}
		}
	}

	void	Proposition::AddElement(const ILogicElement& oElement)
	{
		m_oElements.push_back(oElement.Duplicate());
	}

	void	Proposition::ReplaceAtom(ILogicElement::AtomId iId, const IProposition& oNew)
	{
		for (std::vector< SPtr<ILogicElement> >::iterator itElem = m_oElements.begin(), itEnd = m_oElements.end(); itElem != itEnd; ++itElem)
		{
			if ((*itElem)->GetType() == E_PROPOSITION)
			{
				dynamic_cast<Proposition*>(itElem->Get())->ReplaceAtom(iId, oNew);
			}
			else if ((*itElem)->GetType() == E_ATOM)
			{
				const Atom*	pAtom = dynamic_cast<const Atom*>(itElem->Get());
				if (pAtom->GetId() == iId)
				{
					itElem->Release();
					*itElem = oNew.Duplicate();
				}
			}
		}
	}
}
