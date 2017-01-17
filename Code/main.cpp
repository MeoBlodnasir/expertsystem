
#include "FileReader.h"

#include <vector>

int main(int ac, char *av[])
{
	ft::FileReader	oFileReader;
	
	oFileReader.Read("../Assets/test.txt");

	return 0;
}