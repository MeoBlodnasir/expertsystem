#pragma once

#include "ErrorCode.h"

#include <string>

namespace ft
{
	namespace File
	{
		EErrorCode	Read(std::string* pOutFileContent, const char* csFilePath);
	};
}
