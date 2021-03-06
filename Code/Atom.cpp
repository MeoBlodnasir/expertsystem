
#include "Atom.h"

#include "VariablesManager.h"
#include "Core.h"

namespace ft
{
	Atom::Atom()
		: IProposition()
		, m_iId(0)
	{
	}

	Atom::Atom(Id iId)
		: IProposition()
		, m_iId(iId)
	{
	}

	Atom::Atom(const Atom& oAtom)
		: IProposition(oAtom)
		, m_iId(oAtom.m_iId)
	{
	}

	Atom::~Atom()
	{
	}

	ILogicElement*	Atom::Duplicate() const
	{
		return new Atom(*this);
	}

	bool	Atom::operator == (const ILogicElement& oElem) const 
	{
		const Atom* pAtom = dynamic_cast<const Atom*>(&oElem);
		if (pAtom != nullptr)
		{
			return m_iId == pAtom->m_iId;
		}
		return false;
	}

	bool	Atom::Evaluate(const VariablesManager& oVariablesManager) const
	{
		return oVariablesManager.GetVariable(m_iId)->GetState();
	}

	bool	Atom::CheckValidity() const
	{
		return m_iId != 0;
	}

	void	Atom::GetAtomsId(AtomIdSet* pIdSet) const
	{
		FT_ASSERT(pIdSet != nullptr);

		pIdSet->insert(m_iId);
	}
}
