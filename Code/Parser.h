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

	class Parser
	{
	public:

		struct ParsingData
		{
			std::vector<Rule>*					pRules;
			VariablesManager*					pFacts;
			std::vector<ILogicElement::AtomId>*	pQueries;

			ParsingData() : pRules(nullptr), pFacts(nullptr), pQueries(nullptr)	{}
		};

		Parser();
		~Parser();

		void		ResetParser();
		EErrorCode	ReadTokens(ParsingData* pData, const std::vector<Token>& oTokens);

	private:

		enum ETokenReadingState
		{
			E_UNDEFINED = -1,
			E_RULE,
			E_FACTS,
			E_QUERIES
		};

		enum ECheckTokenStateFlag
		{
			E_NONE					= 0,
			E_WAITFOR_ENTRY			= (1 << 0),
			E_WAITFOR_SYMBOL		= (1 << 1),
			E_WAITFOR_VARIABLE		= (1 << 2),
			E_WAITFOR_OPERATOR		= (1 << 3),
			E_WAITFOR_IMPLY			= (1 << 4),
			E_WAITFOR_EOL_EOF		= (1 << 5),
			E_DONTWAIT_OPEN_PAR		= (1 << 6),
			E_DONTWAIT_CLOSE_PAR	= (1 << 7),
			E_WAITONLY_VARIABLE		= (1 << 8)
		};

		enum ERuleBuildingState
		{
			E_ANTECEDENT,
			E_CONSEQUENT
		};

		// Etats généraux
		ETokenReadingState			m_eReadingState;
		// Etats de CheckToken
		int32						m_iCheckTokenStateFlags;
		int32						m_iCheckTokenParenthesisLevel;
		// Etats de RuleBuilding
		std::stack<const Token*>	m_oPendingElems;
		ERuleBuildingState			m_eRuleBuildingState;

		void	ResetCheckTokenStates();
		bool	CheckToken(const Token& oToken);

		void	ResetBuildRuleStates();
		void	BuildRule(Rule* pRule, const Token& oToken);

		void	AddElementToRule(Rule* pRule, const ILogicElement& oElement);
		void	ForceAddElementToRuleFromToken(Rule* pRule, const Token& oToken);
		void	UnstackPendingElements(Rule* pRule);
	};
}
