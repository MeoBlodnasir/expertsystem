#pragma once

#include "Core.h"

namespace ft
{
	class FileReader
	{
	public:

		FileReader();
		~FileReader();

		EErrorCode	Read(const char* oFilePath) const;

	private:

		FileReader(const FileReader&);
		FileReader& operator=(const FileReader&);
	};
}
