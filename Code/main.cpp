
#include "Core.h"
#include "Application.h"

int		main(int ac, char **av)
{
	ft::Application	oApplication;

	FT_TEST_RETURN(oApplication.Init(ac, av) == ft::FT_OK, -1);
	FT_TEST_RETURN(oApplication.Run() == ft::FT_OK, -1);
	FT_TEST_RETURN(oApplication.Destroy() == ft::FT_OK, -1);

	return 0;
}
