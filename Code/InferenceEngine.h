#pragma once

#include "Rule.h"

#include <vector>

namespace ft
{
	// fw
	class VariablesManager;

	class InferenceEngine
	{
	public:

		InferenceEngine();
		~InferenceEngine();

		std::string	ProcessQuery(const VariablesManager& oFacts, const std::vector<Rule>& oRules, ILogicElement::AtomId iQuery);

	private:

		InferenceEngine(const InferenceEngine&);
		InferenceEngine& operator = (const InferenceEngine&);
	};
}
