
#include "File.h"

#include "Core.h"
#include "Output.h"

#include <fstream>
#include <sstream>

namespace ft
{
	namespace File
	{
		EErrorCode	GetContent(std::string *pOutFileContent, const char* csFilePath)
		{
			FT_ASSERT(pOutFileContent != nullptr);
			FT_ASSERT(csFilePath != nullptr);

			std::ifstream		oIFStream(csFilePath);
			std::stringstream	oBuffer;

			if (oIFStream.rdstate() & std::ifstream::failbit)
			{
				FT_CERR << "Echec dans l'ouverture du fichier " << csFilePath << std::endl;
				return FT_FAIL;
			}

			oBuffer << oIFStream.rdbuf();
			oIFStream.close();
			*pOutFileContent = oBuffer.str();

			return FT_OK;
		}
	}
}
