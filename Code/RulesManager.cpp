
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
		for (const Rule& itRule : m_oRules)
			FT_COUT << itRule << std::endl;
	}

	bool	RulesManager::AddRule(const Rule& oRule)
	{
		// N'ajoute la règle que si elle est valide.
		if (oRule.CheckComponentsValidity())
		{
			if (!oRule.CheckUnacceptedConditions())
			{
				DivideBidirectionnalRule(oRule);
				m_oRules.push_back(oRule);
				DivideRule(m_oRules.end() - 1);
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
			for (Atom::Id iId : pIdSet)
			{
				Rule oNewRule(*itRule);
				oNewRule.SetAntecedentProposition(itRule->GetAntecedent());
				oNewRule.SetConsequentProposition(Proposition(Atom(iId)));
				oNewRules.push_back(oNewRule);
			}
			m_oRules.erase(itRule);
		}
		for (const Rule& itNewRule : oNewRules)
			m_oRules.push_back(itNewRule);
	}
}
