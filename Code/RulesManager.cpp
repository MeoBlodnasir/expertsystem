
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


	const std::vector<Rule>&	RulesManager::GetRules() 
	{
		return m_oRules;
	}

	void				RulesManager::PrintRules()
	{
		FT_COUT << "RULES MANAGER" << std::endl;
		for (std::vector<Rule>::const_iterator itRule = m_oRules.begin(), itEnd = m_oRules.end(); itRule != itEnd; itRule++)
			FT_COUT << *itRule << std::endl;
	}

	bool	RulesManager::AddRule(const Rule& oRule)
	{
		// N'ajoute la règle que si elle est valide.
		bool bIsValid = oRule.CheckComponentsValidity();
		bool bIntegrity = oRule.CheckIntegrity();

		if (bIsValid && bIntegrity)
			m_oRules.push_back(oRule);

		return bIsValid;
	}

	bool	RulesManager::AddRules(const std::vector<Rule>& oRules)
	{
		bool	bRet = true;

		for (std::vector<Rule>::const_iterator itRule = oRules.begin(), itEnd = oRules.end(); itRule != itEnd; ++itRule)
		{
			if (!AddRule(*itRule))
			{
				FT_NOT_IMPLEMENTED("Cas d'erreur d'ajout de regle");
				bRet = false;
			}
		}

		return bRet;
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


	bool	RulesManager::CheckRules()
	{
		for (std::vector<Rule>::const_iterator itRule = m_oRules.begin(); itRule != m_oRules.end(); ++itRule)
		{
			Proposition oConsequent = itRule->GetConsequent();
			Proposition oAntecedent= itRule->GetAntecedent();
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

	}
}
