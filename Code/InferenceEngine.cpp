
#include "InferenceEngine.h"

#include "Types.h"
#include "Core.h"
#include "VariablesManager.h"
#include "Output.h"
#include "LogicOperator.h"
#include "ILogicElement.h"
#include "RulesManager.h"

#include <algorithm>
#include <sstream>

namespace ft
{
	InferenceEngine::InferenceEngine()
		: m_bVerbose(false)
	{
	}

	InferenceEngine::~InferenceEngine()
	{
	}

	ConstantAtom	InferenceEngine::GoalEvaluation(const VariablesManager& oFacts, const RulesManager& oRules, std::vector<ILogicElement::AtomId>* pQueries, ILogicElement::AtomId iQuery) const
	{
		ConstantAtom	oRetAtom(oFacts.GetVariable(iQuery)->GetState());
		bool			bConsequentRuleFound = false;

		pQueries->push_back(iQuery);
		if (m_bVerbose)
			FT_COUT << std::string(pQueries->size() - 1, '\t') << "? " << iQuery << std::endl;

		for (const Rule& itRule : oRules.GetRules())
		{
			if (itRule.GetConsequentFirstAtomId() == iQuery)
			{
				bool		bIsDetermined = false;
				bool		bEvaluation = false;
				AtomIdSet	oAtomSet;
				Proposition	oProposition(itRule.GetAntecedent());

				if (m_bVerbose)
					FT_COUT << std::string(pQueries->size(), '\t')  << "? (" << itRule.GetAntecedent().GetDesc() << ')' << std::endl;
				bConsequentRuleFound = true;
				itRule.GetAntecedent().GetAtomsId(&oAtomSet);
				for (const ILogicElement::AtomId& itAtomId : oAtomSet)
				{
					if (std::find(pQueries->begin(), pQueries->end(), itAtomId) == pQueries->end())
					{
						oProposition.ReplaceAtom(itAtomId, GoalEvaluation(oFacts, oRules, pQueries, itAtomId));
					}
					else
					{
						if (m_bVerbose)
						{
							FT_COUT << std::string(pQueries->size(), '\t') << "? " << itAtomId << std::endl;
							FT_COUT << std::string(pQueries->size() + 1, '\t') << "STOP: " << itAtomId << " = " << oFacts.GetVariable(itAtomId)->GetState() << std::endl;
						}
						oProposition.ReplaceAtom(itAtomId, ConstantAtom(oFacts.GetVariable(itAtomId)->GetState()));
					}
				}
				bIsDetermined = itRule.IsBidirectionnal();
				bEvaluation = oProposition.Evaluate(oFacts);
				if (m_bVerbose)
					FT_COUT << std::string(pQueries->size(), '\t') << '(' << itRule.GetAntecedent().GetDesc() << ')' << " = " << bEvaluation << std::endl;
				if (bEvaluation || bIsDetermined)
				{
					const std::vector< SPtr<ILogicElement> >& oConsequentElements = itRule.GetConsequent().GetElements();
					bool bIsInverted = (oConsequentElements.size() > 1) && (dynamic_cast<const OperatorNOT*>(oConsequentElements[1].Get()) != nullptr);
					oRetAtom = ConstantAtom(bIsInverted ? !bEvaluation : bEvaluation);

					// Maintenant que le NOT est pris en compte,
					// que faire quand plusieurs règles se contedisent à l'exécution?

					break;
				}
			}
		}

		if (!bConsequentRuleFound)
		{
			if (m_bVerbose)
				FT_COUT << std::string(pQueries->size(), '\t') << iQuery << " = " << oFacts.GetVariable(iQuery)->GetState() << std::endl;
		}

		pQueries->pop_back();
		return oRetAtom;
	}

	bool	InferenceEngine::ProcessQuery(const VariablesManager& oFacts, const RulesManager& oRules, ILogicElement::AtomId iQuery) const
	{
		std::vector<ILogicElement::AtomId> oQueries;
		
		return GoalEvaluation(oFacts, oRules, &oQueries, iQuery).GetState();
	}
}
