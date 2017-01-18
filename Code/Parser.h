#pragma once

#include "Token.h"
#include "VariableStorage.h"
#include "Rule.h"

#include <set>

namespace ft
{
	namespace Parser
	{
		struct ParsingResult
		{
			VariableStorage*	pVariableStorage;
			std::vector<Rule>	oRules;
			std::set<char>		oFacts;
			std::set<char>		oQueries;
		};

		EErrorCode	ReadTokens(ParsingResult *pOutParsingResult, const std::vector<Token>& oTokens);
	};
}
