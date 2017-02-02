
#include "Core.h"

#include "Output.h"

namespace ft
{
	namespace Core
	{
		void	PrintAssertInfos(const char* csExpression, const char* csFilePath, const int iLine, const char* csFunction)
		{
			FT_CERR << "################ ASSERT FAILED ################" << std::endl;
			FT_CERR << "Fichier:  " << csFilePath << " (" << iLine << ')' << std::endl;
			FT_CERR << "Fonction: " << csFunction << std::endl;
			FT_CERR << std::endl << csExpression << std::endl << std::endl;
			FT_CERR << "###############################################" << std::endl;
		}
	}
}
