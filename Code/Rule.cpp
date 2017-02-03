
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

	bool	Rule::operator == (const Rule& oRule) const
	{
		return m_oAntecedent == oRule.m_oAntecedent
			/*&& m_bIsBidirectionnal == oRule.m_bIsBidirectionnal*/
			&& m_oConsequent == oRule.m_oConsequent;
	}

	ILogicElement::AtomId	Rule::GetConsequentFirstAtomId() const
	{
		FT_ASSERT(m_oConsequent.GetElements().size() > 0);
		FT_ASSERT(m_oConsequent.GetElements()[0]->GetType() == ILogicElement::E_ATOM);
		return dynamic_cast<const Atom*>(m_oConsequent.GetElements()[0].Get())->GetId();
	}

	void		Rule::AddAntecedentElement(const ILogicElement& oElement)
	{
		m_oAntecedent.AddElement(oElement);
	}

	void		Rule::AddConsequentElement(const ILogicElement& oElement)
	{
		m_oConsequent.AddElement(oElement);
	}

	void		Rule::DeleteNotPairs()
	{
		m_oAntecedent.DeleteNotPairs();
		m_oConsequent.DeleteNotPairs();
	}

	bool		Rule::CheckComponentsValidity() const
	{
		return m_oAntecedent.CheckValidity() && m_oConsequent.CheckValidity();
	}

	bool		Rule::CheckUnacceptedConditions() const
	{
		return
			(	m_oConsequent.IsXorPresent()
			||	m_oConsequent.IsOrPresent()
			|| (m_oConsequent.IsNotPresent() && !m_oConsequent.IsConsequentAcceptedNot())
			)
			||
			(m_bIsBidirectionnal &&
				(	m_oAntecedent.IsXorPresent()
				||	m_oAntecedent.IsOrPresent()
				|| (m_oAntecedent.IsNotPresent() && !m_oAntecedent.IsConsequentAcceptedNot())
				)
			);
	}
}
