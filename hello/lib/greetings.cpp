#include <iostream>
#include "greetings.h"

bool greetings(const char* name)
{
	std::cout << "Hello " << name << "!" << std::endl;
	return std::cout.fail() ? EXIT_FAILURE : EXIT_SUCCESS;
}
