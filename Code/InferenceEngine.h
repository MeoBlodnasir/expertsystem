#pragma once

#include "StrongPointer.h"
#include "Rule.h"

#include <vector>

namespace ft
{
	// fw
	class VariablesManager;

	class InferenceEngine : public CountableSPtr
	{
	public:

		InferenceEngine();
		~InferenceEngine();

		bool	ProcessQuery(const VariablesManager& oFacts, const std::vector<Rule>& oRules, ILogicElement::AtomId iQuery);

	private:

		InferenceEngine(const InferenceEngine&);
		InferenceEngine& operator = (const InferenceEngine&);
	};
}
