#include "sample_module.hpp"
#include <iostream>

bool say(const char* word)
{
	std::cout << word << std::endl;
	return std::cout.fail() ? EXIT_FAILURE : EXIT_SUCCESS;
}
