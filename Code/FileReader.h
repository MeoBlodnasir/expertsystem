#pragma once

#include "ErrorCode.h"
#include "Rule.h"

namespace ft
{
	class FileReader
	{
	public:

		FileReader();
		~FileReader();

		EErrorCode	Read(const char* oFilePath, Rule oRule) const;

	private:

		FileReader(const FileReader&);
		FileReader& operator=(const FileReader&);
	};
}
