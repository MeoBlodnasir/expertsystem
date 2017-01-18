
#include "FileReader.h"

#include "Core.h"
#include "Output.h"
#include "Lexer.h"
#include "Parser.h"

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

	EErrorCode	FileReader::Read(const char* csFilePath, Rule oRule) const
	{
		FT_ASSERT(csFilePath != nullptr);

		std::ifstream		oIFStream(csFilePath);
		std::stringstream	oStringStream;
		std::vector<Token>	oTokens;

		if (oIFStream.rdstate() & std::ifstream::failbit)
		{
			FT_CERR << "Echec dans l'ouverture du fichier " << csFilePath << std::endl;
			return FT_FAIL;
		}

		oStringStream << oIFStream.rdbuf();
		oIFStream.close();

		FT_TEST_OK(Lexer::ReadInput(&oTokens, oStringStream.str().c_str()));

		for (std::vector<Token>::const_iterator it = oTokens.begin(), itEnd = oTokens.end(); it != itEnd; ++it)
		{
			oRule.AddConditionElement(Parser::CheckTokenType(*it));

		}

		return FT_OK;
	}
}
