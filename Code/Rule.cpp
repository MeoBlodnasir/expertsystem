
#include "Rule.h"

#include "Core.h"
#include "Atom.h"
#include "Proposition.h"

#include <unordered_set>

namespace ft
{
	Rule::Rule()
		: m_bIsBidirectionnal(false)
	{
	}

	Rule::Rule(const Rule& oRule)
		: m_oAntecedent(oRule.m_oAntecedent)
		, m_oConsequent(oRule.m_oConsequent)
		, m_bIsBidirectionnal(oRule.m_bIsBidirectionnal)
	{
	}

	Rule::~Rule()
	{
	}

	ILogicElement::AtomId	Rule::GetConsequentAtomId() const
	{
		AtomIdSet oAtomsId;

		m_oConsequent.GetAtomsId(&oAtomsId);

		FT_ASSERT(oAtomsId.size() > 0);
		return *oAtomsId.begin();
	}

	void		Rule::AddAntecedentElement(const ILogicElement& oElement)
	{
		m_oAntecedent.AddElement(oElement);
	}

	void		Rule::AddConsequentElement(const ILogicElement& oElement)
	{
		m_oConsequent.AddElement(oElement);
	}

	bool		Rule::CheckComponentsValidity() const
	{
		return m_oAntecedent.CheckValidity() && m_oConsequent.CheckValidity();
	}

	bool		Rule::CheckIntegrity() const
	{
		AtomIdSet oAntecedentAtomsId;
		AtomIdSet oConsequentAtomsId;
		GetAntecedent().GetAtomsId(&oAntecedentAtomsId);
		GetConsequent().GetAtomsId(&oConsequentAtomsId);
		int iOriginalSize = oAntecedentAtomsId.size() + oConsequentAtomsId.size();
		oAntecedentAtomsId.insert(oConsequentAtomsId.begin(), oConsequentAtomsId.end());
		int iFinalSize = oAntecedentAtomsId.size();
		if (iOriginalSize != iFinalSize)
			return false;
		return true;
	}
}
