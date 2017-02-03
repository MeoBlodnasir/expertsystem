#pragma once

#include "StrongPointer.h"
#include "ErrorCode.h"
#include "Rule.h"

#include <map>

namespace ft
{
	class RulesManager : public CountableSPtr
	{
	public:

		typedef std::vector<Rule>	RulesSet;

		class ContradictionException : public std::exception
		{
		public:
			ContradictionException(const Rule& _oRule = Rule()) : oRule(_oRule) {}
			Rule oRule;
		};

		RulesManager();
		~RulesManager();

		bool			DoesExistRuleThatImplies(ILogicElement::AtomId iId) const;
		const RulesSet&	GetRulesThatImply(ILogicElement::AtomId iId) const;

		void	Flush();
		bool	AddRule(const Rule& oRule, bool bCheckBidirectionnal = true);
		void	PrintRules() const;

	private:

		std::map<ILogicElement::AtomId, RulesSet>	m_oRules;

		void	DivideBidirectionnalRule(const Rule& oRule);
		bool	DivideRule(const Rule& oRule);
		bool	CheckDuplications(const Rule& oRule, ILogicElement::AtomId iVectorKey) const;
		bool	CheckContradictions(const Rule& oRule, ILogicElement::AtomId iVectorKey) const;
	};
}
