#pragma once

#include "ErrorCode.h"

#include <string>

namespace ft
{
	namespace File
	{
		EErrorCode	GetContent(std::string* pOutFileContent, const char* csFilePath);
	};
}
