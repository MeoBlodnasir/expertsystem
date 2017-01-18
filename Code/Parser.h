#pragma once

#include "Token.h"
#include "RuleStack.h"

namespace ft
{
	namespace Parser
	{
		IRuleElement	*CheckTokenType(Token token);
		
	};
}
