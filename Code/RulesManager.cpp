
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
			FT_COUT << " => ";
			itRule->GetConsequent().Print();
			FT_COUT << std::endl;
		}

	}

	void	RulesManager::AddRule(const Rule& oRule)
	{
		m_oRules.push_back(oRule);
	}

	void				RulesManager::DivideRules()
	{
		std::vector<Rule> oNewRules;
		for (std::vector<Rule>::const_iterator itRule = m_oRules.begin(); itRule != m_oRules.end(); )
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
					itRule = m_oRules.erase(itRule);

			}
				else
					itRule++;
		}
		for (std::vector<Rule>::const_iterator itRule = oNewRules.begin(); itRule != oNewRules.end();itRule++ )
		{
			m_oRules.push_back(*itRule);

		}


	}
}
