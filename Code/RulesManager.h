#pragma once

#include "StrongPointer.h"
#include "ErrorCode.h"
#include "Rule.h"

namespace ft
{
	class RulesManager : public CountableSPtr
	{
		public:

			RulesManager();
			~RulesManager();

			const std::vector<Rule>&	GetRules();
			bool						AddRule(const Rule& oRule);
			bool						AddRules(const std::vector<Rule>& oRules);
			void						DivideRules();
			void						PrintRules();

		private:
			
			std::vector<Rule>	m_oRules;

			void				DivideBidirectionnalRules();
	};
}
