
#include "Core.h"

#include "Output.h"

namespace ft
{
	namespace Core
	{
		void	PrintAssertInfos(const char* csExpression, const char* csFilePath, const int iLine, const char* csFunction)
		{
			FT_COUT << "################ ASSERT FAILED ################" << std::endl;
			FT_COUT << "Fichier:  " << csFilePath << " (" << iLine << ')' << std::endl;
			FT_COUT << "Fonction: " << csFunction << std::endl;
			FT_COUT << std::endl << csExpression << std::endl << std::endl;
			FT_COUT << "###############################################" << std::endl;
		}
	}
}
