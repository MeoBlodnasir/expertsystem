#pragma once

#include "ErrorCode.h"
#include "Rule.h"

namespace ft
{
	class RulesManager
	{
		public:

			RulesManager();
			~RulesManager();

			const std::vector<Rule>&	GetRules();
			bool						AddRule(const Rule& oRule);
			bool						CheckRules();
			void						DivideRules();
			void						PrintRules();

		private:
			
			std::vector<Rule>	m_oRules;

			void				DivideBidirectionnalRules();
	};
}
