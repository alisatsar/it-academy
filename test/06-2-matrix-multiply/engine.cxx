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
#include <tuple>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#include "picopng.hxx"

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
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;

class texture_es20 final : public my_texture
{
private:
	GLuint tex_handler = 0;
	std::string tex_path;
	unsigned long tex_width = 0;
	unsigned long tex_height = 0;
public:
	~texture_es20() override;
	unsigned long get_width() const;
	unsigned long get_height() const;

	explicit texture_es20(std::string_view path);
};

texture_es20::~texture_es20()
{
    glDeleteTextures(1, &tex_handler);
}

texture_es20::texture_es20(std::string_view path) : tex_path(path)
{
	std::vector<unsigned char> png_file_in_memory;

	std::ifstream ifs(path.data(), std::ios_base::binary);
	if (!ifs)
	{
		throw std::runtime_error("texture_es20::texture_es20::texture is not loaded\n");
	}

	ifs.seekg(0, std::ios_base::end);
	size_t pos_in_file = ifs.tellg();
	png_file_in_memory.resize(pos_in_file);
	ifs.seekg(0, std::ios_base::beg);
	if (!ifs)
	{
		throw std::runtime_error("texture_es20::texture_es20::texture is not loaded\n");
	}

	ifs.read(reinterpret_cast<char*>(png_file_in_memory.data()),
			    pos_in_file);

	if (!ifs.good())
	{
		throw std::runtime_error("texture_es20::texture_es20::texture is not loaded\n");
	}

	std::vector<unsigned char> image;
	tex_width = 0;
	tex_height = 0;
	int error = decodePNG(image, tex_width, tex_height, &png_file_in_memory[0],
				png_file_in_memory.size(), false);

	if (error != 0)
	{
		throw std::runtime_error("texture_es20::texture_es20::texture is not loaded\n");
	}

	glGenTextures(1, &tex_handler);

	glBindTexture(GL_TEXTURE_2D, tex_handler);

	GLint mipmap_level = 0;
	GLint border = 0;
	glTexImage2D(GL_TEXTURE_2D, mipmap_level, GL_RGBA, tex_width, tex_height, border,
					GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

unsigned long texture_es20::get_width() const
{
	return tex_width;
}

unsigned long texture_es20::get_height() const
{
	return tex_width;
}

my_texture::~my_texture(){}

class shader
{
private:
	 GLuint vert_shader = 0;
	 GLuint frag_shader = 0;
	 GLuint program_id  = 0;
private:
	 GLuint compile_shader(GLenum shader_type, std::string_view src)
	 {
		 GLuint shader_id = glCreateShader(shader_type);

		 std::string_view shader_src = src;
		 const GLchar* sh_source = shader_src.data();

		 glShaderSource(shader_id, 1, &sh_source, NULL);
		 glCompileShader(shader_id);

		 GLint  success;
		 char infoLog[512];
		 glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
		 if(!success)
		 {
			 glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
			 std::cout << "ERROR::SHADER::compile_shader::COMPILATION_FAILED\n" << infoLog << std::endl;
		 }
		 return shader_id;
	 }

	 GLuint link_shader_program(const std::vector<std::tuple<GLuint, const GLchar*>>& attributes)
	 {
		 GLuint program_id_ = glCreateProgram();

		 if (0 == program_id_)
		 {
		     throw std::runtime_error("can't link shader");
		 }

		 glAttachShader(program_id_, vert_shader);

		 glAttachShader(program_id_, frag_shader);

		 for (const auto& attr : attributes)
		 {
			 GLuint loc  = std::get<0>(attr);
			 const GLchar* name = std::get<1>(attr);
			 glBindAttribLocation(program_id_, loc, name);
		 }

		 glLinkProgram(program_id_);

		 GLint linked_status = 0;
		 glGetProgramiv(program_id_, GL_LINK_STATUS, &linked_status);

		 if (linked_status == 0)
		 {
			 	 GLint infoLen = 0;
			 	 glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &infoLen);

			 	 std::vector<char> infoLog(static_cast<size_t>(infoLen));
			 	 glGetProgramInfoLog(program_id_, infoLen, NULL, infoLog.data());

			 	 std::cerr << "Error linking program:\n" << infoLog.data();
			 	 glDeleteProgram(program_id_);

			 	 return 0;
		 }
		 return program_id_;
	 }
public:
	 shader(std::string_view vertex_src, std::string_view fragment_src,
			 const std::vector<std::tuple<GLuint, const GLchar*>>& attributes)
	{
		 vert_shader = compile_shader(GL_VERTEX_SHADER, vertex_src);
		 frag_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_src);
		 if (vert_shader == 0 || frag_shader == 0)
		 {
		     throw std::runtime_error("can't compile shader");
		 }

		 program_id = link_shader_program(attributes);
		 if (program_id == 0)
		 {
			 throw std::runtime_error("can't link shader");
		 }
	}
	 void use()
	 {
		 glUseProgram(program_id);

		 	    //return the location of a uniform variable
		 	    int location = glGetUniformLocation(program_id, "s_texture");
		 	    assert(-1 != location);


		 	    //select active texture unit
		 	    int texture_unit = 0;
		 	    glActiveTexture(GL_TEXTURE0 + texture_unit);

		 	    glUniform1i(location, 0 + texture_unit);
		 	    glEnable(GL_BLEND);

		 	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 }
};

using namespace math;



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

	shader* shader02 = nullptr;

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

		load_functions();
		return true;
	}

	void load_functions()
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

	void create_my_shader(my_texture* t) final
	{
		shader02 = new shader(
		        R"(
attribute vec2 a_position;
attribute vec2 a_texcoord;
varying vec2 v_texcoord;
void main()
{
    v_texcoord = a_texcoord;
    gl_Position = vec4(a_position, 0.0, 1.0);
}
		                )",
		        R"(
precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D s_texture;
void main()
{
    gl_FragColor = texture2D(s_texture, v_texcoord);
}
		                )",
	        { { 0, "a_position" }, { 1, "a_texcoord" } });

		shader02->use();
	}

	void render_triangle(triangle& t) final
	{
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &t.vert[0].vect.x);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &t.vert[0].tex_vect.x);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_TRIANGLES, 1, 3);
	}

	float get_time_from_init() final
	{
		std::uint32_t tick_ms = SDL_GetTicks();
		float seconds = tick_ms * 0.001f;
		return seconds;
	}

	my_texture* load_texture(std::string_view path) final
	{
		return new texture_es20(path);
	}

	void destroy_texture(my_texture* t) final
	{
		delete t;
	}

	math::vertex blend_vertex(const math::vertex& vl, const math::vertex& vr,
	                        const float a)
	{
		math::vertex r;
	    r.vect.x = (1.0f - a) * vl.vect.x + a * vr.vect.x;
	    r.vect.y = (1.0f - a) * vl.vect.y + a * vr.vect.y;
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

	math::triangle create_tex_triangles(float size, size_t count_sprites_line, bool right) final
	{
		math::triangle t;
		float tex = 1.f / count_sprites_line;
		if (right)
		{
			t.vert[0].vect.x = -size;
			t.vert[0].vect.y = -size;
			t.vert[0].tex_vect.x = 0.0;
			t.vert[0].tex_vect.y = 0.0;
			t.vert[1].vect.x = size;
			t.vert[1].vect.y = -size;
			t.vert[1].tex_vect.x = tex;
			t.vert[1].tex_vect.y = 0.0;
			t.vert[2].vect.x = size;
			t.vert[2].vect.y = size;
			t.vert[2].tex_vect.x = tex;
			t.vert[2].tex_vect.y = tex;
		}
		else
		{
			t.vert[0].vect.x = size;
			t.vert[0].vect.y = size;
			t.vert[0].tex_vect.x = tex;
			t.vert[0].tex_vect.y = tex;
			t.vert[1].vect.x = -size;
			t.vert[1].vect.y = size;
			t.vert[1].tex_vect.x = 0.0;
			t.vert[1].tex_vect.y = tex;
			t.vert[2].vect.x = -size;
			t.vert[2].vect.y = -size;
			t.vert[2].tex_vect.x = 0.0;
			t.vert[2].tex_vect.y = 0.0;
		}
		return t;
	}

	void start_animate(math::triangle& right_t, math::triangle& left_t, float sec) final
	{
		static float a = 0.f;
		static float b = 0.f;

		static float left = 0.f;
		static float right = 0.2f;

		if((a - b) >= sec)
		{
			if(left < 0.8)
			{
				left += 0.2;
			}
			else
			{
				left = 0.0;
			}
			if(right < 1.f)
			{
				right += 0.2;
			}
			else
			{
				right = 0.2;
			}

			right_t.vert[0].tex_vect.x = left;
			left_t.vert[1].tex_vect.x = left;
			left_t.vert[2].tex_vect.x = left;

			right_t.vert[1].tex_vect.x = right;
			right_t.vert[2].tex_vect.x = right;
			left_t.vert[0].tex_vect.x = right;

			b = a;
		}

		a = get_time_from_init();
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
