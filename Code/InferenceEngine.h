#pragma once

#include "StrongPointer.h"
#include "Rule.h"
#include "Atom.h"

#include <vector>
#include <exception>

namespace ft
{
	// fw
	class VariablesManager;
	class RulesManager;

	class InferenceEngine : public CountableSPtr
	{
	public:

		class ContradictionException : public std::exception {};

		InferenceEngine();
		~InferenceEngine();

		inline void		SetVerbose(bool bVerbose)		{ m_bVerbose = bVerbose; }

		bool			ProcessQuery(const VariablesManager& oFacts, const RulesManager& oRules, ILogicElement::AtomId iQuery) const;

	private:

		bool	m_bVerbose;

		InferenceEngine(const InferenceEngine&);
		InferenceEngine& operator = (const InferenceEngine&);
	
		ConstantAtom	GoalEvaluation(const VariablesManager& oFacts, const RulesManager& oRules, std::vector<ILogicElement::AtomId>* pQueries, ILogicElement::AtomId iQuery) const;
	};
}
