
#include "RulesManager.h"

#include "Output.h"
#include "Atom.h"
#include "ILogicElement.h"

namespace ft
{
	RulesManager::RulesManager()
	{
	}

	RulesManager::~RulesManager()
	{
	}


	void	RulesManager::Reset()
	{
		m_oRules.clear();
	}

	void	RulesManager::PrintRules() const
	{
		FT_COUT << "RULES MANAGER" << std::endl;
		for (std::vector<Rule>::const_iterator itRule = m_oRules.begin(), itEnd = m_oRules.end(); itRule != itEnd; itRule++)
			FT_COUT << *itRule << std::endl;
	}

	bool	RulesManager::AddRule(const Rule& oRule)
	{
		// N'ajoute la règle que si elle est valide.
		bool bIsValid = oRule.CheckComponentsValidity();

		if (bIsValid)
		{
			DivideBidirectionnalRule(oRule);
			m_oRules.push_back(oRule);
			DivideRule(m_oRules.end() - 1);
		}

		return bIsValid;
	}

	void	RulesManager::DivideBidirectionnalRule(const Rule& oRule)
	{
		if (oRule.IsBidirectionnal())
		{
			Rule oNewRule(oRule);
			oNewRule.SetAntecedentProposition(oRule.GetConsequent());
			oNewRule.SetConsequentProposition(oRule.GetAntecedent());
			m_oRules.push_back(oNewRule);
		}
	}
	
	void	RulesManager::DivideRule(std::vector<Rule>::const_iterator itRule)
	{
		std::vector<Rule>	oNewRules;
		AtomIdSet			pIdSet;

		itRule->GetConsequent().GetAtomsId(&pIdSet);
		if (pIdSet.size() > 1)
		{
			for (std::unordered_set<Atom::Id>::iterator itSet = pIdSet.begin(); itSet != pIdSet.end(); itSet++)
			{
				Rule oNewRule(*itRule);
				oNewRule.SetAntecedentProposition(itRule->GetAntecedent());
				oNewRule.SetConsequentProposition(Proposition(Atom(*itSet)));
				oNewRules.push_back(oNewRule);
			}
			m_oRules.erase(itRule);
		}
		for (std::vector<Rule>::const_iterator itNewRule = oNewRules.begin(); itNewRule != oNewRules.end();itNewRule++ )
			m_oRules.push_back(*itNewRule);
	}


	bool	RulesManager::CheckRules() const
	{
		for (std::vector<Rule>::const_iterator itRule = m_oRules.begin(); itRule != m_oRules.end(); ++itRule)
		{
			const Proposition& oConsequent = itRule->GetConsequent();
			const Proposition& oAntecedent = itRule->GetAntecedent();
			if (oConsequent.XorPresent() || oConsequent.OrPresent() || oConsequent.NotPresent())
			{
				FT_COUT << "Xor, Or or Not present in Consequent of a rule" << std::endl;
				return false;
			}
			if (itRule->IsBidirectionnal() && (oAntecedent.XorPresent() || oAntecedent.OrPresent() || oAntecedent.NotPresent()))
			{
				FT_COUT << "Xor, Or or Not present in Antecedent of a bidirectional rule" << std::endl;
				return false;
			}
		}
		return true;
	}
}
