
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


	std::vector<Rule>	RulesManager::GetRules() 
	{
		return m_oRules;
	}

	void				RulesManager::PrintRules()
	{
		for (std::vector<Rule>::const_iterator itRule = m_oRules.begin(), itEnd = m_oRules.end(); itRule != itEnd; itRule++)
		{
			itRule->GetAntecedent().Print();
			if (itRule->IsBidirectionnal())
				FT_COUT << " <=> ";
			else
				FT_COUT << " => ";
			itRule->GetConsequent().Print();
			FT_COUT << std::endl;
		}

	}

	void	RulesManager::AddRule(const Rule& oRule)
	{
		m_oRules.push_back(oRule);
	}

	void				RulesManager::DivideBidirectionnalRules()
	{
		std::vector<Rule> oNewRules;

		for (std::vector<Rule>::iterator itRule = m_oRules.begin(); itRule != m_oRules.end(); itRule++)
		{
			if (itRule->IsBidirectionnal())
			{
				Rule oRule;
				oRule.AddAntecedentElement(itRule->GetConsequent());
				oRule.AddConsequentElement(itRule->GetAntecedent());
				oNewRules.push_back(oRule);
				itRule->SetBidirectionnal(false);
			}
		}
		for (std::vector<Rule>::iterator itNewRule = oNewRules.begin(); itNewRule != oNewRules.end();itNewRule++ )
			m_oRules.push_back(*itNewRule);

	}

	void				RulesManager::DivideRules()
	{
		DivideBidirectionnalRules();
		std::vector<Rule> oNewRules;
		for (std::vector<Rule>::const_iterator itRule = m_oRules.begin(); itRule != m_oRules.end(); ++itRule)
		{
			AtomIdSet pIdSet;
			itRule->GetConsequent().GetAtomsId(&pIdSet);
			if (pIdSet.size() > 1)
			{
				for (std::unordered_set<Atom::Id>::iterator itSet = pIdSet.begin(); itSet != pIdSet.end(); itSet++)
				{
					Rule oRule;
					oRule.AddAntecedentElement(itRule->GetAntecedent());
					oRule.AddConsequentElement(Atom(*itSet));
					oNewRules.push_back(oRule);
				}

			}
			else
			{
				Rule oRule;
				oRule.AddAntecedentElement(itRule->GetAntecedent());
				oRule.AddConsequentElement(itRule->GetConsequent());
				oNewRules.push_back(oRule);
			}

		}
		m_oRules.clear();
		for (std::vector<Rule>::const_iterator itNewRule = oNewRules.begin(); itNewRule != oNewRules.end();itNewRule++ )
			m_oRules.push_back(*itNewRule);
	}
}
