#include "engine.hxx"

#include <algorithm>
#include <array>
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
//#include <SDL2/SDL_opengles2.h>

#include "picopng.hxx"

using namespace math;

PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLACTIVETEXTUREPROC glActiveTexture_ = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;

template<class T>
void get_func_pointer(const char* func_name, T& result)
{
	///to get an OpenGL function by name
	void* gl_pointer(SDL_GL_GetProcAddress(func_name));
	if(gl_pointer == nullptr)
	{
		throw std::runtime_error("Can't load gl function");
	}
	result = reinterpret_cast<T>(gl_pointer);
}

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

	void clear_color() final
	{
		glClearColor(0.f, 1.0, 0.f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void swap_buffers() final
	{
		SDL_GL_SwapWindow(window);
        glClearColor(0.f, 1.0, 0.f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
	}

	void unintialize() final
	{
		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void create_my_shader() final
	{
		try
		{
			get_func_pointer("glGenBuffers", glGenBuffers);
			get_func_pointer("glBindBuffer", glBindBuffer);
			get_func_pointer("glBufferData", glBufferData);
			get_func_pointer("glCreateShader", glCreateShader);
			get_func_pointer("glShaderSource", glShaderSource);
			get_func_pointer("glCompileShader", glCompileShader);
			get_func_pointer("glGetShaderiv", glGetShaderiv);
			get_func_pointer("glGetShaderInfoLog", glGetShaderInfoLog);
			get_func_pointer("glCreateProgram", glCreateProgram);
			get_func_pointer("glAttachShader", glAttachShader);
			get_func_pointer("glGetProgramiv", glGetProgramiv);
			get_func_pointer("glGetProgramInfoLog", glGetProgramInfoLog);
			get_func_pointer("glLinkProgram", glLinkProgram);
			get_func_pointer("glUseProgram", glUseProgram);
			get_func_pointer("glDeleteShader", glDeleteShader);
			get_func_pointer("glVertexAttribPointer", glVertexAttribPointer);
			get_func_pointer("glBindAttribLocation", glBindAttribLocation);
			get_func_pointer("glEnableVertexAttribArray", glEnableVertexAttribArray);
			get_func_pointer("glGetUniformLocation", glGetUniformLocation);
			get_func_pointer("glActiveTexture", glActiveTexture_);
			get_func_pointer("glUniform1i", glUniform1i);
		}
		catch(std::exception& ex)
		{
			ex.what();
		}
		///create a shader object
		GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* vertex_shader_src = R"(
attribute vec2 a_position;
attribute vec2 a_texcoord;
varying vec2 v_texcoord;
void main()
{
    v_texcoord = a_texcoord;
    gl_Position = vec4(a_position, 0.0, 1.0);
})";
		///replace the source code in a shader object
		///what replace		count element	string source shader
		glShaderSource(vert_shader, 1, &vertex_shader_src, NULL);
		/// compile a shader object
		glCompileShader(vert_shader);

		GLint  success;
		char infoLog[512];
		///returns shader parametr
		glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
		    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		///CREATE FRAGMENT SHADER
		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fragment_shader_src = R"(
precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D s_texture;
void main()
{
    gl_FragColor = texture2D(s_texture, v_texcoord);
})";
		glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
		glCompileShader(fragment_shader);

		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		///now lets binding our shaders
		///creating a program object
		GLuint shader_program = glCreateProgram();
		if (0 == shader_program)
		{
		    std::cout << "failed to create gl program";
		    glDeleteShader(vert_shader);
		    glDeleteShader(fragment_shader);
		}
		///attach a shader object to a program object
		glAttachShader(shader_program, vert_shader);
		glAttachShader(shader_program, fragment_shader);
		///create an executable that will run on the programmable vertex processor
		glBindAttribLocation(shader_program, 0, "a_position");
		glLinkProgram(shader_program);
		GLint program_linked;
		glGetProgramiv(shader_program, GL_LINK_STATUS, &program_linked);
		if (program_linked != GL_TRUE)
		{
		    GLsizei log_length = 0;
		    GLchar message[1024];
		    glGetProgramInfoLog(shader_program, 1024, &log_length, message);
		    std::cout << "ERROR::LINKED IS_FAILED\n" << message << std::endl;
		}

		///install the program as current rendering state
		glUseProgram(shader_program);

	    //return the location of a uniform variable
	    int location = glGetUniformLocation(shader_program, "s_texture");
	    assert(-1 != location);

	    int texture_unit = 0;
	    //select active texture unit
	    glActiveTexture(GL_TEXTURE0 + texture_unit);
	    if (!load_texture("boy-320.png"))
	    {
	        std::cout << "failed load texture\n";
	    }

	    glUniform1i(location, 0 + texture_unit);
	    glEnable(GL_BLEND);

	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void render_triangle(triangle& t) final
	{
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &t.vert[0].vec.x);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &t.vert[0].text_vec.x);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_TRIANGLES, 1, 3);
	}

	float get_time_from_init() final
	{
		std::uint32_t tick_ms = SDL_GetTicks();
		float seconds = tick_ms * 0.001f;
		return seconds;
	}

	bool load_texture(std::string_view path) final
	{
		//создаем массив чаров
		std::vector<unsigned char> png_file_in_memory;
		//path.data() - вернет указатель на первый символ
		// open file for reading в бинарном режиме
		std::ifstream ifs(path.data(), std::ios_base::binary);
		if (!ifs)
		{
			return false;
		}
		//перемещаем курсор смещение в конец
		ifs.seekg(0, std::ios_base::end);
		//узнаем значение курсора, это и есть наш размер
		size_t pos_in_file = ifs.tellg();
		//перевыделяем память в контейнере, равном нашему размеру
		png_file_in_memory.resize(pos_in_file);
		//перемещаем курсор смещение в начало
		ifs.seekg(0, std::ios_base::beg);
		if (!ifs)
		{
			return false;
		}

		//извлекаем символы из потока
		ifs.read(reinterpret_cast<char*>(png_file_in_memory.data()),
		    pos_in_file);
		//вернет истину, если последняя операция в потоке ввода\вывода
		//завершилась успешно
		if (!ifs.good())
		{
		    return false;
		}

		std::vector<unsigned char> image;
		unsigned long w = 0;
		unsigned long h = 0;
		int error = decodePNG(image, w, h, &png_file_in_memory[0],
				png_file_in_memory.size(), false);

		if (error != 0)
		{
		    std::cerr << "error: " << error << std::endl;
		    return false;
		}

		GLuint tex_handl = 0;
		glGenTextures(1, &tex_handl);

		glBindTexture(GL_TEXTURE_2D, tex_handl);

		GLint mipmap_level = 0;
		GLint border = 0;
		glTexImage2D(GL_TEXTURE_2D, mipmap_level, GL_RGBA, w, h, border,
				GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return true;
	}

	math::vertex blend_vertex(const math::vertex& vl, const math::vertex& vr,
	                        const float a)
	{
		math::vertex r;
	    r.vec.x = (1.0f - a) * vl.vec.x + a * vr.vec.x;
	    r.vec.y = (1.0f - a) * vl.vec.y + a * vr.vec.y;
	    return r;
	}

	math::triangle blend(const math::triangle& tl, const math::triangle& tr,
	                   const float a)
	{
		math::triangle r;
	    r.vert[0] = blend_vertex(tl.vert[0], tr.vert[0], a);
	    r.vert[1] = blend_vertex(tl.vert[1], tr.vert[1], a);
	    r.vert[2] = blend_vertex(tl.vert[2], tr.vert[2], a);
	    return r;
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
