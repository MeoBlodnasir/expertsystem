
#include "InferenceEngine.h"

#include "Types.h"
#include "Core.h"
#include "VariablesManager.h"
#include "Output.h"
#include "LogicOperator.h"
#include "ILogicElement.h"

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
			if (itRule->GetConsequentAtomId() == oId)
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
}
