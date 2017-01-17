#pragma once

#include "Core.h"
#include "Token.h"

#include <vector>

namespace ft
{
	namespace Lexer
	{
		EErrorCode	ReadInput(std::vector<Token>* pTokens, const char* csInput);
	};
}
