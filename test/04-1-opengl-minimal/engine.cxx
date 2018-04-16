#include "engine.hxx"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string_view>

#include <SDL2/SDL.h>

//#define GLFW_INCLUDE_ES2
//#include <GLFW/glfw3.h>
#include <SDL2/SDL_opengles2.h>

struct bind
{
	SDL_Keycode key;
	std::string_view name;
	te::event pressed;
	te::event released;
};

const std::array<bind, 8> keys{
    { { SDLK_w, "up", te::event::up_pressed, te::event::up_released },
      { SDLK_a, "left", te::event::left_pressed, te::event::left_released },
      { SDLK_s, "down", te::event::down_pressed, te::event::down_released },
      { SDLK_d, "right", te::event::right_pressed, te::event::right_released },
      { SDLK_LCTRL, "button1", te::event::button_a_pressed,
    		  te::event::button_a_pressed },
      { SDLK_SPACE, "button2", te::event::button_b_pressed,
    		  te::event::button_b_pressed },
      { SDLK_ESCAPE, "select", te::event::select_pressed, te::event::select_released },
      { SDLK_RETURN, "start", te::event::start_pressed, te::event::start_released } }
};

bool check_input(const SDL_Event& e, const bind*& result)
{
	auto it = std::find_if(std::begin(keys), std::end(keys),[&](const ::bind& b) {
	        return b.key == e.key.keysym.sym;
		} );
	///функция find_if вернет указатель на итератора за последним,
	///если не удовлетворит условию поиска ламбда выражения
	if(it != std::end(keys))
	{
		result = it;
		return true;
	}
	return true;
}

class my_tiny_engine : public te::engine
{
private:
	SDL_GLContext gl_context;
	SDL_Window* window = nullptr;

public:
	std::string check_version() final
	{
		std::string s;
		SDL_version compiled;
		SDL_version linked;

		SDL_VERSION(&compiled);
		SDL_GetVersion(&linked);

		bool result = SDL_VERSIONNUM(compiled.major, compiled.minor, compiled.patch) ==
				SDL_VERSIONNUM(linked.major, linked.minor, linked.patch);

		if(!result)
		{
			s = "WARNING: The compiled version is not equal linked version!";
		}
		else
		{
			s = "SUCCESS: The compiled version is equal linked version!";
		}
		return s;
	}

	bool initialize() final
	{
		const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
		if(init_result != 0)
		{
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return false;
		}
		return true;
	}

	bool create_window(const char* title, int pos_x, int pos_y, int width, int height)
	{
		window = SDL_CreateWindow(
				title,
				pos_x,
				pos_y,
				height,
				width,
				SDL_WINDOW_OPENGL
				);

		if(window == NULL)
		{
			SDL_Log("Could not create window: %s\n", SDL_GetError());
			SDL_Quit();
			return false;
		}


		///create context for window (for usage function opengl)
		gl_context = SDL_GL_CreateContext(window);
		if(gl_context == NULL)
		{
			std::cout << "GLContext error";
		}
		///if return 0, then a message is written to the standard error, terminating the program execution
		assert(gl_context != nullptr);

		int gl_major_ver = 0;
		///SDL_GL_GetAttribute - actual value for an attribute from the current context
		///SDL_GL_CONTEXT_MAJOR_VERSION - context major version
		///return 0 if SUCCESS
		int result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);
		assert (result == 0);
		int gl_minor_ver = 0;
		int result2 = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_ver);
		assert (result2 == 0);

		if (gl_major_ver <= 2 && gl_minor_ver >= 1)
		{
			std::clog << "current context opengl version: " << gl_major_ver
			    << '.' << gl_minor_ver << '\n'<< "need opengl version at least: 2.1\n"
			                      << std::flush;
			throw std::runtime_error("opengl version too low");
		}
		return true;
	}

	bool read_input(te::event& e)
	{
		SDL_Event sdl_event;

		///Returns 1 if there is a pending event or 0 if there are none available.
		if(SDL_PollEvent(&sdl_event))
		{
			const bind* binding = nullptr;

			if(sdl_event.type == SDL_QUIT)
			{
				e = te::event::turn_off;
				return true;
			}
			else if(sdl_event.type == SDL_KEYUP)
			{
				if(check_input(sdl_event, binding))
				{
					e = binding->released;
					return true;
				}
			}
			else if(sdl_event.type == SDL_KEYDOWN)
			{
				if(check_input(sdl_event, binding))
				{
					e = binding->pressed;
					return true;
				}
			}
		}
		return false;
	}

	void clear_color(const te::triangle& t) final
	{
		glClearColor(0.f, 1.0, 0.f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void swap_buffers() final
	{
		SDL_GL_SwapWindow(window);
	}

	void unintialize() final
	{
		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
};

te::engine* te::create_engine()
{
	return new my_tiny_engine();
}

void te::destroy_engine(te::engine* e)
{
	delete e;
}

te::engine::~engine(){}
