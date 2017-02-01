
#include "InferenceEngine.h"

#include "Types.h"
#include "Core.h"
#include "VariablesManager.h"
#include "Output.h"
#include "LogicOperator.h"
#include "ILogicElement.h"
#include "RulesManager.h"

#include <algorithm>

namespace ft
{
	InferenceEngine::InferenceEngine()
	{
	}

	InferenceEngine::~InferenceEngine()
	{
	}

	std::vector<const Proposition*>	GetSubGoalPropositions(const std::vector<Rule>& oRules, ILogicElement::AtomId oId)
	{
		std::vector<const Proposition*> oPropositions;
		for (std::vector<Rule>::const_iterator itRule = oRules.begin(), itEnd = oRules.end(); itRule != itEnd; ++itRule)
		{
			if (itRule->GetConsequentFirstAtomId() == oId)
			{
				oPropositions.push_back(&(itRule->GetAntecedent()));
			}
		}
		return oPropositions;
	}

	std::string						InferenceEngine::ProcessQuery(const VariablesManager& oVariablesManager, const std::vector<Rule>& oRules, ILogicElement::AtomId iQuery)
	{
		bool isFinished = false;
		Proposition MasterProposition;
		std::map<ILogicElement::AtomId, Proposition> mReplacements;
		MasterProposition.AddElement(Atom(iQuery));

		while (!isFinished)
		{
			isFinished = true;
			AtomIdSet oAtomSet;
			MasterProposition.GetAtomsId(&oAtomSet);
			for (AtomIdSet::const_iterator itAtomId = oAtomSet.begin(), itEnd = oAtomSet.end(); itAtomId != itEnd; ++itAtomId)
			{
				std::vector<const Proposition*> oPropositions = GetSubGoalPropositions(oRules, *itAtomId);
				if (oPropositions.size() > 0)
				{
					Proposition	SubGoalPropostion;

					for (std::vector<const Proposition*>::const_iterator itProposition = oPropositions.begin(), itPropositionEnd = oPropositions.end(); itProposition != itPropositionEnd; ++itProposition)
						SubGoalPropostion.AddElement(**itProposition);

					for (uint32 i = 1; i < oPropositions.size(); ++i)
						SubGoalPropostion.AddElement(OperatorOR());
					if (mReplacements.find(*itAtomId) == mReplacements.end())
					{
						MasterProposition.ReplaceAtom(*itAtomId, SubGoalPropostion);
						mReplacements[*itAtomId] = SubGoalPropostion;
						isFinished = false;
					}
					else
						return "Query is dependant of a loop in rules";
				}
			}
		}

		return MasterProposition.Evaluate(oVariablesManager) ? "true" : "false";
	}

	ConstantAtom	GoalEvaluation(const VariablesManager& oFacts, const RulesManager& oRules, std::vector<ILogicElement::AtomId>* pQueries, ILogicElement::AtomId iQuery)
	{
		ConstantAtom	oRetAtom(oFacts.GetVariable(iQuery)->GetState());
		bool			bConsequentRuleFound = false;
		pQueries->push_back(iQuery);
		FT_COUT << std::string(pQueries->size() - 1, '\t') << "? " << iQuery << std::endl;

		for (const Rule& itRule : oRules.GetRules())
		{
			if (itRule.GetConsequentFirstAtomId() == iQuery)
			{
				bool		bIsDetermined = false;
				bool		bEvaluation = false;
				AtomIdSet	oAtomSet;
				Proposition	oProposition(itRule.GetAntecedent());

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
						FT_COUT << std::string(pQueries->size(), '\t') << "? " << itAtomId << std::endl;
						FT_COUT << std::string(pQueries->size() + 1, '\t') << "STOP: " << itAtomId << " = " << oFacts.GetVariable(itAtomId)->GetState() << std::endl;
						oProposition.ReplaceAtom(itAtomId, ConstantAtom(oFacts.GetVariable(itAtomId)->GetState()));
					}
				}
				bIsDetermined = itRule.IsBidirectionnal();
				bEvaluation = oProposition.Evaluate(oFacts);
				FT_COUT << std::string(pQueries->size(), '\t') << '(' << itRule.GetAntecedent().GetDesc() << ')' << " = " << bEvaluation << std::endl;
				if (bEvaluation || bIsDetermined)
				{
					oRetAtom = ConstantAtom(bEvaluation);
					break;
				}
			}
		}

		if (!bConsequentRuleFound)
		{
			FT_COUT << std::string(pQueries->size(), '\t') << iQuery << " = " << oFacts.GetVariable(iQuery)->GetState() << std::endl;
		}

		pQueries->pop_back();
		return oRetAtom;
	}

	bool	InferenceEngine::NewProcessQuery(const VariablesManager& oFacts, const RulesManager& oRules, ILogicElement::AtomId iQuery)
	{
		std::vector<ILogicElement::AtomId> oQueries;
		
		return GoalEvaluation(oFacts, oRules, &oQueries, iQuery).GetState();
	}
}
