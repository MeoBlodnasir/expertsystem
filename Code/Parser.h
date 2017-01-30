#pragma once

#include "Types.h"
#include "Rule.h"
#include "Token.h"
#include "ILogicElement.h"

#include <stack>
#include <vector>

namespace ft
{
	//fw
	class VariablesManager;
	class RulesManager;

	class Parser
	{
	public:

		struct OutData
		{
			enum EType
			{
				E_NONE,
				E_RULE,
				E_FACTS,
				E_QUERIES,
				E_COMMAND
			};

			EType				eDataType;
			Token::EType		eCommandType;
			std::vector<Rule>	oRules;
			AtomIdSet			oAtoms;
		};

		Parser();
		~Parser();

		EErrorCode	ReadTokens(OutData* pData, const std::vector<Token>& oTokens);

	private:

		enum ERuleBuildingState
		{
			E_ANTECEDENT,
			E_CONSEQUENT
		};

		ERuleBuildingState		m_eRuleState;

		EErrorCode	ReadRule(OutData* pData, const std::vector<Token>& oTokens);
		void		AddElementToRule(Rule* pRule, const ILogicElement& oElement);
		void		AddTokenToRule(Rule* pRule, const Token& oToken);
		void		UnstackPendingElements(std::stack<const Token*>* pPendingTokens, Rule* pRule);

		EErrorCode	ReadFacts(OutData* pData, const std::vector<Token>& oTokens);

		EErrorCode	ReadQueries(OutData* pData, const std::vector<Token>& oTokens);

		EErrorCode	ReadCommand(OutData* pData, const std::vector<Token>& oTokens);
	};
}
