#pragma once

#include "ErrorCode.h"
#include "Token.h"

#include <vector>

namespace ft
{
	namespace Lexer
	{
		struct OutData
		{
			std::vector<Token>	oTokens;
		};

		EErrorCode	ReadLine(OutData* pLexingData, const std::string& sLine);
	};
}
