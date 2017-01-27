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
			void	AddRule(const Rule& oRule);
			std::vector<Rule>	GetRules() ;
			void				DivideRules();
			void				PrintRules();


		private:

			std::vector<Rule>	m_oRules;
	};
}
