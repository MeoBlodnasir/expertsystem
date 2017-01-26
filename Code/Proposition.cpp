
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
		for (std::vector<ILogicElement*>::const_iterator itElem = oProposition.m_oElements.begin(), itEnd = oProposition.m_oElements.end();
			itElem != itEnd; ++itElem)
		{
			m_oElements.push_back((*itElem)->Duplicate());
		}
	}

	Proposition::~Proposition()
	{
		for (std::vector<ILogicElement*>::iterator itElem = m_oElements.begin(), itEnd = m_oElements.end();
			itElem != itEnd; ++itElem)
		{
			FT_SAFE_DELETE(*itElem);
		}
	}

	ILogicElement*	Proposition::Duplicate() const
	{
		return new Proposition(*this);
	}

	bool	Proposition::Evaluate(const VariablesManager* pVariablesManager) const
	{
		FT_ASSERT(SelfAssert());

		std::stack<bool> oResultStack;

		for (std::vector<ILogicElement*>::const_iterator itElem = m_oElements.begin(), itEnd = m_oElements.end(); itElem != itEnd; ++itElem)
		{
			if ((*itElem)->GetType() == E_PROPOSITION || (*itElem)->GetType() == E_ATOM)
			{
				bool bState = dynamic_cast<const IProposition*>(*itElem)->Evaluate(pVariablesManager);
				oResultStack.push(bState);
			}
			else if ((*itElem)->GetType() == ILogicElement::E_OPERATOR)
			{
				const uint32 iOperandCount = dynamic_cast<const ALogicOperator*>(*itElem)->iOperandCount;

				if (iOperandCount == 1)
				{
					oResultStack.top() = dynamic_cast<const AUnaryOperator*>(*itElem)->Evaluate(oResultStack.top());
				}
				else if (iOperandCount == 2)
				{
					bool bTemp = oResultStack.top();
					oResultStack.pop();
					oResultStack.top() = dynamic_cast<const ABinaryOperator*>(*itElem)->Evaluate(bTemp, oResultStack.top()); // vérifier pour l'ordre (1, 2) ou (2, 1)
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

	bool	Proposition::SelfAssert() const
	{
		return true;
	}

	void	Proposition::GetAtomsId(AtomIdSet* pIdSet) const
	{
		FT_ASSERT(pIdSet != nullptr);

		for (std::vector<ILogicElement*>::const_iterator itElem = m_oElements.begin(), itEnd = m_oElements.end(); itElem != itEnd; ++itElem)
		{
			if ((*itElem)->GetType() == E_PROPOSITION)
			{
				dynamic_cast<const Proposition*>(*itElem)->GetAtomsId(pIdSet);
			}
			else if ((*itElem)->GetType() == E_ATOM)
			{
				dynamic_cast<const Atom*>(*itElem)->GetAtomsId(pIdSet);
			}
		}
	}

	void	Proposition::AddElement(const ILogicElement& oElement)
	{
		m_oElements.push_back(oElement.Duplicate());
	}

	void	Proposition::ReplaceAtom(ILogicElement::AtomId iId, const IProposition& oNew)
	{
		for (std::vector<ILogicElement*>::iterator itElem = m_oElements.begin(), itEnd = m_oElements.end(); itElem != itEnd; ++itElem)
		{
			if ((*itElem)->GetType() == E_PROPOSITION)
			{
				dynamic_cast<Proposition*>(*itElem)->ReplaceAtom(iId, oNew);
			}
			else if ((*itElem)->GetType() == E_ATOM)
			{
				const Atom*	pAtom = dynamic_cast<const Atom*>(*itElem);
				if (pAtom->GetId() == iId)
				{
					FT_SAFE_DELETE(*itElem);
					*itElem = oNew.Duplicate();
				}
			}
		}
	}
}
