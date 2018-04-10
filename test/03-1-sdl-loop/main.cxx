#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <string_view>

#include <SDL2/SDL.h>

std::ostream& operator<<(std::ostream& out, const SDL_version version)
{
	out << version.major << ".";
	out << version.minor << ".";
	out << version.patch;
	return out;
}

bool check_version()
{
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	return SDL_VERSIONNUM(compiled.major, compiled.minor, compiled.patch) ==
			SDL_VERSIONNUM(linked.major, linked.minor, linked.patch);
}

struct bind
{
	SDL_Keycode key;
	std::string_view name;
};

void check_input(const SDL_Event& e)
{
	std::array<bind, 8> keys { { {SDLK_w, "up"},
		{SDLK_a, "right"},
		{SDLK_s, "down"},
		{SDLK_d, "left"},
		{SDLK_LCTRL, "button_A"},
		{SDLK_SPACE, "button_B"},
		{SDLK_ESCAPE, "select"},
		{SDLK_RETURN, "start"} } };

	const auto it = std::find_if(std::begin(keys), std::end(keys),[&](const ::bind& b) {
        return b.key == e.key.keysym.sym;
	} );

	//функция find_if вернет указатель на итератора за последним,
	//если не удовлетворит условию поиска ламбда выражения
	if(it != std::end(keys))
	{
		//если тип эвента равен нажата клавиша
		if(e.type == SDL_KEYDOWN)
		{
			std::cout << it->name << " is pressed";
		}
		else
		{
			std::cout << it->name << " is released\n";
		}
	}
}

int main()
{
	if(!check_version())
	{
		std::cout << "WARNING: The compiled version is not equal linked version!";
	}

	const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
	if(init_result != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window* const window = SDL_CreateWindow(
			"Title",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			640,
			480,
			SDL_WINDOW_OPENGL
			);

	if(window == NULL)
	{
		std::cout << "Could not create window: %s\n", SDL_GetError();
		SDL_Quit();
		return EXIT_FAILURE;
	}

	bool continue_loop = true;
	while(continue_loop)
	{
		SDL_Event sdl_event;

		while(SDL_PollEvent(&sdl_event))
		{
			switch(sdl_event.type)
			{
			case SDL_KEYDOWN:
				check_input(sdl_event);
				break;
			case SDL_KEYUP:
				check_input(sdl_event);
				break;
			case SDL_QUIT:
				continue_loop = false;
				break;
			default:
				break;
			}
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return EXIT_SUCCESS;
}
