
#include "FileReader.h"

#include "Lexer.h"

#include <fstream>
#include <sstream>

namespace ft
{
	FileReader::FileReader()
	{
	}

	FileReader::~FileReader()
	{
	}

	EErrorCode	FileReader::Read(const char* oFilePath) const
	{
		std::ifstream		oIFStream(oFilePath);
		std::stringstream	oStringStream;
		std::vector<Token>	oTokens;

		if (oIFStream.rdstate() & std::ifstream::failbit)
			return FT_FAIL;

		oStringStream << oIFStream.rdbuf();
		oIFStream.close();

		if (Lexer::ReadInput(&oTokens, oStringStream.str().c_str()) != FT_OK)
			return FT_FAIL; 

		return FT_OK;
	}
}
