
#include "InferenceEngine.h"

#include "Core.h"
#include "VariablesManager.h"
#include "Output.h"

namespace ft
{
	InferenceEngine::InferenceEngine()
	{
	}

	InferenceEngine::~InferenceEngine()
	{
	}

	bool	InferenceEngine::ProcessQuery(const VariablesManager& /*oFacts*/, const std::vector<Rule>& /*oRules*/, ILogicElement::AtomId /*iQuery*/)
	{
		return true;
	}
}
