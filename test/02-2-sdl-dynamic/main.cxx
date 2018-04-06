#include <cstdlib>
#include <iostream>

#include <SDL2/SDL_version.h>

std::ostream& operator<<(std::ostream& out, const SDL_version& v)
{
	out << static_cast<int>(v.major) << ".";
	out << static_cast<int>(v.minor) << ".";
	out << static_cast<int>(v.patch);
	return out;
}

int main()
{
	SDL_version compiler = { 0, 0, 0 };
	SDL_version linker = { 0, 0, 0 };

	SDL_VERSION(&compiler);
	SDL_GetVersion(&linker);

	std::cout << "compiled: " << compiler << "\n";
	std::cout << "linked: " << linker << std::endl;

	bool is_good = std::cout.good();

	int result = is_good ? EXIT_SUCCESS : EXIT_FAILURE;
	return result;
}
