
#include "RulesManager.h"

#include "Output.h"
#include "Atom.h"
#include "ILogicElement.h"
#include "LogicOperator.h"

namespace ft
{
	RulesManager::RulesManager()
	{
	}

	RulesManager::~RulesManager()
	{
	}

	bool	RulesManager::DoesExistRuleThatImplies(ILogicElement::AtomId iId) const
	{
		return m_oRules.count(iId) > 0;
	}

	const RulesManager::RulesSet&	RulesManager::GetRulesThatImply(ILogicElement::AtomId iId) const
	{
		return m_oRules.at(iId);
	}

	void	RulesManager::Flush()
	{
		m_oRules.clear();
	}

	void	RulesManager::PrintRules() const
	{
		FT_COUT << "REGLES" << std::endl;
		for (const std::pair< ILogicElement::AtomId, RulesSet >& itRules : m_oRules)
		{
			for (const Rule& itRule : itRules.second)
				FT_COUT << itRule << std::endl;
		}
	}

	bool	RulesManager::AddRule(const Rule& oRule, bool bCheckBidirectionnal /*= true*/)
	{
		// N'ajoute la règle que si elle est valide.
		bool bAdd = false;

		if (oRule.CheckComponentsValidity())
		{
			if (!oRule.CheckUnacceptedConditions())
			{
				if (bCheckBidirectionnal)
					DivideBidirectionnalRule(oRule);

				if (!DivideRule(oRule))
				{
					ILogicElement::AtomId iConsequentFirstAtomId = oRule.GetConsequentFirstAtomId();
					if (!CheckDuplications(oRule, iConsequentFirstAtomId)
						&& !CheckContradictions(oRule, iConsequentFirstAtomId))
					{
						m_oRules[iConsequentFirstAtomId].push_back(oRule);
						bAdd = true;
					}
				}
				else
					bAdd = true;
			}
			else
			{
				FT_CERR << "Regle non acceptee: " << oRule << std::endl;
			}
		}
		else
		{
			FT_CERR << "Regle invalide: " << oRule << std::endl;
		}

		return bAdd;
	}

	void	RulesManager::DivideBidirectionnalRule(const Rule& oRule)
	{
		if (oRule.IsBidirectionnal())
		{
			Rule oNewRule(oRule);
			oNewRule.SetAntecedentProposition(oRule.GetConsequent());
			oNewRule.SetConsequentProposition(oRule.GetAntecedent());
			AddRule(oNewRule, false);
		}
	}
	
	bool	RulesManager::DivideRule(const Rule& oRule)
	{
		std::vector<Rule>	oNewRules;
		uint32				iConsequentAtomCount = 0;

		const std::vector< SPtr<ILogicElement> >& oConsequentElements = oRule.GetConsequent().GetElements();

		for (std::vector< SPtr<ILogicElement> >::const_iterator itConsequentElement = oConsequentElements.begin(), itEnd = oConsequentElements.end();
			itConsequentElement != itEnd; ++itConsequentElement)
		{
			if ((*itConsequentElement)->GetType() == ILogicElement::E_ATOM)
			{
				Rule oNewRule(oRule);
				oNewRule.SetConsequentProposition(Proposition(*dynamic_cast<const Atom*>(itConsequentElement->Get())));
				if (itConsequentElement + 1 != itEnd)
				{
					if (dynamic_cast<const OperatorNOT*>((itConsequentElement + 1)->Get()) != nullptr)
					{
						oNewRule.AddConsequentElement(OperatorNOT());
						++itConsequentElement;
					}
				}
				oNewRules.push_back(oNewRule);
				++iConsequentAtomCount;
			}
		}

		if (iConsequentAtomCount > 1)
		{
			for (const Rule& itNewRule : oNewRules)
				AddRule(itNewRule, false);
		}

		return iConsequentAtomCount > 1;
	}

	bool	RulesManager::CheckDuplications(const Rule& oRule, ILogicElement::AtomId iVectorKey) const
	{
		bool bDuplication = false;
		if (m_oRules.count(iVectorKey) > 0)
		{
			for (const Rule& itRule : m_oRules.at(iVectorKey))
			{
				if (itRule == oRule)
				{
					bDuplication = true;
					break;
				}
			}
		}
		return bDuplication;
	}

	bool	RulesManager::CheckContradictions(const Rule& oRule, ILogicElement::AtomId iVectorKey) const
	{
		bool bContradiction = false;
		if (m_oRules.count(iVectorKey) > 0)
		{
			Rule oOppositeRule(oRule);
			oOppositeRule.AddConsequentElement(OperatorNOT());
			oOppositeRule.DeleteNotPairs();
			for (const Rule& itRule : m_oRules.at(iVectorKey))
			{
				if (itRule == oOppositeRule)
				{
					bContradiction = true;
					throw ContradictionException(itRule);
					break;
				}
			}
		}
		return bContradiction;
	}

}
