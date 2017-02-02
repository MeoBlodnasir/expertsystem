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

			RulesManager();
			~RulesManager();

			bool						DoesExistRuleThatImplies(ILogicElement::AtomId iId) const;
			const std::vector<Rule>&	GetRulesThatImply(ILogicElement::AtomId iId) const;

			void	Flush();
			bool	AddRule(const Rule& oRule);
			void	PrintRules() const;

		private:
			
			std::map< ILogicElement::AtomId, std::vector<Rule> >	m_oRules;

			void	DivideBidirectionnalRule(const Rule& oRule);
			void	DivideRule(std::vector<Rule>::iterator itRule, ILogicElement::AtomId iVectorKey);
			bool	CheckContradictions(ILogicElement::AtomId iVectorKey) const;
	};
}
