
#if defined(_WIN32)
#	include <crtdbg.h>
#endif

#include "Core.h"
#include "Application.h"

FT_TODO("Voir comment traquer les fuites mémoire sur Linux et Mac aussi.");

int		main(int ac, char **av)
{
	// Traque des fuites mémoire
#if defined(_WIN32)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	ft::Application	oApplication;

	FT_TEST_RETURN(oApplication.Init() == ft::FT_OK, -1);
	FT_TEST_RETURN(oApplication.Run(ac, av) == ft::FT_OK, -1);
	FT_TEST_RETURN(oApplication.Destroy() == ft::FT_OK, -1);

	return 0;
}
