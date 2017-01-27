
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

	bool	Atom::Evaluate(const VariablesManager& oVariablesManager) const
	{
		return oVariablesManager.GetVariable(m_iId)->GetState();
	}

	bool	Atom::SelfAssert() const
	{
		return true;
	}

	void	Atom::GetAtomsId(std::unordered_set<Id>* pIdSet) const
	{
		FT_ASSERT(pIdSet != nullptr);

		pIdSet->insert(m_iId);
	}
}
