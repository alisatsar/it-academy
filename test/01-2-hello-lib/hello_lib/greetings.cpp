/*
 * greetings.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: alisatsar
 */

#include <iostream>
#include "greetings.hpp"

bool greetings(const char* name)
{
	std::cout << name << std::endl;
	return std::cout.fail() ? EXIT_FAILURE : EXIT_SUCCESS;
}
