
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
		: sLitteralExpression(oRule.sLitteralExpression)
		, m_oAntecedent(oRule.m_oAntecedent)
		, m_oConsequent(oRule.m_oConsequent)
		, m_bIsBidirectionnal(oRule.m_bIsBidirectionnal)
	{
	}

	Rule::~Rule()
	{
	}

	ILogicElement::AtomId	Rule::GetConsequentFirstAtomId() const
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
		// Vérifie que la règle ne fait pas référence à un même Atom des deux côtés de son implication
		AtomIdSet oAntecedentAtomsId;
		AtomIdSet oConsequentAtomsId;
		m_oAntecedent.GetAtomsId(&oAntecedentAtomsId);
		m_oConsequent.GetAtomsId(&oConsequentAtomsId);
		uint32 iOriginalSize = oAntecedentAtomsId.size() + oConsequentAtomsId.size();
		oAntecedentAtomsId.insert(oConsequentAtomsId.begin(), oConsequentAtomsId.end());
		uint32 iFinalSize = oAntecedentAtomsId.size();

		return iOriginalSize == iFinalSize;
	}
}
