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

			void						Reset();

			const std::vector<Rule>&	GetRules();
			bool						AddRule(const Rule& oRule);
			bool						CheckRules() const;
			void						DivideRules();
			void						PrintRules() const;

		private:
			
			std::vector<Rule>	m_oRules;

			void				DivideBidirectionnalRules();
	};
}
