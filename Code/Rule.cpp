
#include "Rule.h"

#include "Core.h"
#include "Atom.h"
#include "Proposition.h"

#include <unordered_set>

namespace ft
{
	Rule::Rule()
	{
	}

	Rule::Rule(const Rule& oRule)
		: m_oAntecedent(oRule.m_oAntecedent)
		, m_oConsequent(oRule.m_oConsequent)
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
}
