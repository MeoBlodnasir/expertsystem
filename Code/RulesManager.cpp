
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

	const std::vector<Rule>&	RulesManager::GetRulesThatImply(ILogicElement::AtomId iId) const
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
		for (const std::pair< ILogicElement::AtomId, std::vector<Rule> >& itRules : m_oRules)
		{
			for (const Rule& itRule : itRules.second)
				FT_COUT << itRule << std::endl;
		}
	}

	bool	RulesManager::AddRule(const Rule& oRule)
	{
		// N'ajoute la règle que si elle est valide.
		if (oRule.CheckComponentsValidity())
		{
			if (!oRule.CheckUnacceptedConditions())
			{
				ILogicElement::AtomId	iConsequentFirstAtomId;

				DivideBidirectionnalRule(oRule);
				iConsequentFirstAtomId = oRule.GetConsequentFirstAtomId();
				m_oRules[iConsequentFirstAtomId].push_back(oRule);
				DivideRule(m_oRules[iConsequentFirstAtomId].end() - 1, iConsequentFirstAtomId);

				// Vérifier les contradictions entre règles
				// ex: a=>b		a=>!b

				return true;
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

		return false;
	}

	void	RulesManager::DivideBidirectionnalRule(const Rule& oRule)
	{
		if (oRule.IsBidirectionnal())
		{
			Rule					oNewRule(oRule);
			ILogicElement::AtomId	iConsequentFirstAtomId;
			oNewRule.SetAntecedentProposition(oRule.GetConsequent());
			oNewRule.SetConsequentProposition(oRule.GetAntecedent());
			iConsequentFirstAtomId = oNewRule.GetConsequentFirstAtomId();
			m_oRules[iConsequentFirstAtomId].push_back(oNewRule);
			DivideRule(m_oRules[iConsequentFirstAtomId].end() - 1, iConsequentFirstAtomId);
		}
	}
	
	void	RulesManager::DivideRule(std::vector<Rule>::const_iterator itRule, ILogicElement::AtomId iVectorKey)
	{
		std::vector<Rule>	oNewRules;
		uint32				iConsequentAtomCount = 0;

		const std::vector< SPtr<ILogicElement> >& oConsequentElements = itRule->GetConsequent().GetElements();

		for (std::vector< SPtr<ILogicElement> >::const_iterator itConsequentElement = oConsequentElements.begin(), itEnd = oConsequentElements.end();
			itConsequentElement != itEnd; ++itConsequentElement)
		{
			if ((*itConsequentElement)->GetType() == ILogicElement::E_ATOM)
			{
				Rule oNewRule(*itRule);
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
			ILogicElement::AtomId	iConsequentFirstAtomId;

			m_oRules[iVectorKey].erase(itRule);
			for (const Rule& itNewRule : oNewRules)
			{
				iConsequentFirstAtomId = itNewRule.GetConsequentFirstAtomId();
				m_oRules[iConsequentFirstAtomId].push_back(itNewRule);
			}
		}
	}
}
