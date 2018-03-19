#include <iostream>

int main()
{
	std::cout << "Hello world!" << std::endl;
	return std::cout.fail() ? EXIT_FAILURE : EXIT_SUCCESS;
}
