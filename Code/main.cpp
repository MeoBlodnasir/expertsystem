
#include "FileReader.h"

#include <vector>
#include <iostream> //

int main(int ac, char *av[])
{
	ft::FileReader	oFileReader;
	
	std::cout << (oFileReader.Read("../Assets/test.txt") == ft::FT_OK ? "FT_OK" : "FT_FAIL") << std::endl;

	return 0;
}