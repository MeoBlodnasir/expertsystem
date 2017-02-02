
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


	void	RulesManager::Flush()
	{
		m_oRules.clear();
	}

	void	RulesManager::PrintRules() const
	{
		FT_COUT << "REGLES" << std::endl;
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
			Rule oNewRule(oRule);
			oNewRule.SetAntecedentProposition(oRule.GetConsequent());
			oNewRule.SetConsequentProposition(oRule.GetAntecedent());
			m_oRules.push_back(oNewRule);
			DivideRule(m_oRules.end() - 1);
		}
	}
	
	void	RulesManager::DivideRule(std::vector<Rule>::const_iterator itRule)
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
			m_oRules.erase(itRule);
			for (const Rule& itNewRule : oNewRules)
				m_oRules.push_back(itNewRule);
		}
	}
}
