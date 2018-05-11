#include "engine.hxx"

#include <atomic>
#include <algorithm>
#include <array>
#include <cassert>
#include <iterator>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string_view>
#include <thread>
#include <tuple>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#include "picopng.hxx"
#include "color.hxx"

static PFNGLCREATESHADERPROC             glCreateShader             = nullptr;
static PFNGLSHADERSOURCEPROC             glShaderSource             = nullptr;
static PFNGLCOMPILESHADERPROC            glCompileShader            = nullptr;
static PFNGLGETSHADERIVPROC              glGetShaderiv              = nullptr;
static PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog         = nullptr;
static PFNGLDELETESHADERPROC             glDeleteShader             = nullptr;
static PFNGLCREATEPROGRAMPROC            glCreateProgram            = nullptr;
static PFNGLATTACHSHADERPROC             glAttachShader             = nullptr;
static PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation       = nullptr;
static PFNGLLINKPROGRAMPROC              glLinkProgram              = nullptr;
static PFNGLGETPROGRAMIVPROC             glGetProgramiv             = nullptr;
static PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog        = nullptr;
static PFNGLDELETEPROGRAMPROC            glDeleteProgram            = nullptr;
static PFNGLUSEPROGRAMPROC               glUseProgram               = nullptr;
static PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer      = nullptr;
static PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray  = nullptr;
static PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
static PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation       = nullptr;
static PFNGLUNIFORM1IPROC                glUniform1i                = nullptr;
static PFNGLACTIVETEXTUREPROC            glActiveTexture_           = nullptr;
static PFNGLUNIFORM4FVPROC               glUniform4fv               = nullptr;
static PFNGLUNIFORMMATRIX3FVPROC         glUniformMatrix3fv         = nullptr;

template <typename T>
static void load_gl_func(const char* func_name, T& result)
{
    void* gl_pointer = SDL_GL_GetProcAddress(func_name);
    if (nullptr == gl_pointer)
    {
        throw std::runtime_error(std::string("can't load GL function") +
                                 func_name);
    }
    result = reinterpret_cast<T>(gl_pointer);
}

#define MY_GL_CHECK()                                                          \
    {                                                                          \
        const unsigned int err = glGetError();                                 \
        if (err != GL_NO_ERROR)                                                \
        {                                                                      \
            std::cerr << __FILE__ << '(' << __LINE__ - 1 << ") error: ";       \
            switch (err)                                                       \
            {                                                                  \
                case GL_INVALID_ENUM:                                          \
                    std::cerr << "GL_INVALID_ENUM" << std::endl;               \
                    break;                                                     \
                case GL_INVALID_VALUE:                                         \
                    std::cerr << "GL_INVALID_VALUE" << std::endl;              \
                    break;                                                     \
                case GL_INVALID_OPERATION:                                     \
                    std::cerr << "GL_INVALID_OPERATION" << std::endl;          \
                    break;                                                     \
                case GL_INVALID_FRAMEBUFFER_OPERATION:                         \
                    std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION"            \
                              << std::endl;                                    \
                    break;                                                     \
                case GL_OUT_OF_MEMORY:                                         \
                    std::cerr << "GL_OUT_OF_MEMORY" << std::endl;              \
                    break;                                                     \
            }                                                                  \
            assert(false);                                                     \
        }                                                                      \
    }

std::ostream& operator<<(std::ostream& stream, const input_data& data)
{
	static const std::array<std::string_view, 8> key_names = {
			{
					"left", "right", "up", "down", "select",
					"start", "button1", "button2"
			}
	};

	const std::string_view& key_name = key_names[static_cast<size_t>(data.key)];

	stream << "key: " << key_name << " is_down: " << data.is_down;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const hardware_data& data)
{
	stream << "reset console: " << data.is_reset;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const event& e)
{
	switch(e.type)
	{
	case event_type::input_key:
		stream << std::get<input_data>(e.info);
		break;
	case event_type::hardware:
		stream << std::get<hardware_data>(e.info);
		break;
	}
	return stream;
}

///CLASS TEXTURE IMPLEMENTION///

texture::~texture(){}

class texture_gl_es20 final : public texture
{
private:
	std::string file_path;
	GLuint tex_handler;
	std::uint32_t width = 0;
	std::uint32_t height = 0;
public:
	explicit texture_gl_es20(std::string_view path);
	~texture_gl_es20() override;

	void bind() const
	{
		GLboolean is_texture = glIsTexture(tex_handler);
		assert(is_texture);
		MY_GL_CHECK();
		glBindTexture(GL_TEXTURE_2D, tex_handler);
		MY_GL_CHECK();
	}

	std::uint32_t get_width() const final { return width; }
	std::uint32_t get_height() const final { return height; }
};

texture_gl_es20::texture_gl_es20(std::string_view path)
    : file_path(path)
{
    std::vector<unsigned char> png_file_in_memory;
    std::ifstream ifs(path.data(), std::ios_base::binary);
    if (!ifs)
    {
        throw std::runtime_error("can't load texture");
    }
    ifs.seekg(0, std::ios_base::end);
    std::streamoff pos_in_file = ifs.tellg();
    png_file_in_memory.resize(static_cast<size_t>(pos_in_file));
    ifs.seekg(0, std::ios_base::beg);
    if (!ifs)
    {
        throw std::runtime_error("can't load texture");
    }

    ifs.read(reinterpret_cast<char*>(png_file_in_memory.data()), pos_in_file);
    if (!ifs.good())
    {
        throw std::runtime_error("can't load texture");
    }

    const png_image img = decode_png_file_from_memory(
        png_file_in_memory, convert_color::to_rgba32,
        origin_point::bottom_left);

    // if there's an error, display it
    if (img.error != 0)
    {
        std::cerr << "error: " << img.error << std::endl;
        throw std::runtime_error("can't load texture");
    }

    glGenTextures(1, &tex_handler);
    MY_GL_CHECK();
    glBindTexture(GL_TEXTURE_2D, tex_handler);
    MY_GL_CHECK();

    GLint   mipmap_level = 0;
    GLint   border       = 0;
    GLsizei width        = static_cast<GLsizei>(img.width);
    GLsizei height       = static_cast<GLsizei>(img.height);
    glTexImage2D(GL_TEXTURE_2D, mipmap_level, GL_RGBA, width, height, border,
                 GL_RGBA, GL_UNSIGNED_BYTE, &img.raw_image[0]);
    MY_GL_CHECK();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    MY_GL_CHECK();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    MY_GL_CHECK();
}

texture_gl_es20::~texture_gl_es20()
{
    glDeleteTextures(1, &tex_handler);
    MY_GL_CHECK();
}

///CLASS VBO IMPLEMENSION///
vbo::~vbo() {}
class vbo_impl final : public vbo
{
public:
	vbo_impl(const tri2* tri, std::size_t n)
        : triangles(n)
    {
        assert(tri != nullptr);
        for (size_t i = 0; i < n; ++i)
        {
            triangles[i] = tri[i];
        }
    }
    ~vbo_impl() final;

    const v2*      data() const final { return &triangles.data()->v[0]; }
    virtual size_t size() const final { return triangles.size() * 3; }

private:
    std::vector<tri2> triangles;
};

vbo_impl::~vbo_impl() {}

///SOUND_BUFFER IMPLEMENSION///
static std::string_view get_sound_format_name(uint16_t format_value)
{
    static const std::map<uint16_t, std::string_view> format = {
        { AUDIO_U8, "AUDIO_U8" },         { AUDIO_S8, "AUDIO_S8" },
        { AUDIO_U16LSB, "AUDIO_U16LSB" }, { AUDIO_S16LSB, "AUDIO_S16LSB" },
        { AUDIO_U16MSB, "AUDIO_U16MSB" }, { AUDIO_S16MSB, "AUDIO_S16MSB" },
        { AUDIO_S32LSB, "AUDIO_S32LSB" }, { AUDIO_S32MSB, "AUDIO_S32MSB" },
        { AUDIO_F32LSB, "AUDIO_F32LSB" }, { AUDIO_F32MSB, "AUDIO_F32MSB" },
    };

    auto it = format.find(format_value);
    return it->second;
}

static std::size_t get_sound_format_size(uint16_t format_value)
{
    static const std::map<uint16_t, std::size_t> format = {
        { AUDIO_U8, 1 },     { AUDIO_S8, 1 },     { AUDIO_U16LSB, 2 },
        { AUDIO_S16LSB, 2 }, { AUDIO_U16MSB, 2 }, { AUDIO_S16MSB, 2 },
        { AUDIO_S32LSB, 4 }, { AUDIO_S32MSB, 4 }, { AUDIO_F32LSB, 4 },
        { AUDIO_F32MSB, 4 },
    };

    auto it = format.find(format_value);
    return it->second;
}

class sound_buffer_impl final : public sound
{
public:
    sound_buffer_impl(std::string_view path, SDL_AudioDeviceID device,
                      SDL_AudioSpec audio_spec);
    ~sound_buffer_impl() final;

    void play(const effect prop) final
    {
        // Lock callback function
        SDL_LockAudioDevice(device);

        // here we can change properties
        // of sound and dont collade with multithreaded playing
        current_index = 0;
        is_playing_   = true;
        is_looped     = (prop == effect::looped);

        SDL_UnlockAudioDevice(device);
    }
    bool is_playing() const final { return is_playing_; }
    void stop() final
    {
        // Lock callback function
        SDL_LockAudioDevice(device);

        // here we can change properties
        // of sound and dont collade with multithreaded playing
        current_index = 0;
        is_playing_   = false;
        is_looped     = false;

        // unlock callback for continue mixing of audio
        SDL_UnlockAudioDevice(device);
    }

    std::unique_ptr<uint8_t[]> tmp_buf;
    uint8_t*                   buffer;
    uint32_t                   length;
    uint32_t                   current_index = 0;
    SDL_AudioDeviceID          device;
    bool                       is_playing_ = false;
    bool                       is_looped   = false;
};

sound_buffer_impl::sound_buffer_impl(std::string_view  path,
                                     SDL_AudioDeviceID device_,
                                     SDL_AudioSpec     device_audio_spec)
    : buffer(nullptr)
    , length(0)
    , device(device_)
{
    SDL_RWops* file = SDL_RWFromFile(path.data(), "rb");
    if (file == nullptr)
    {
        throw std::runtime_error(std::string("can't open audio file: ") +
                                 path.data());
    }

    // freq, format, channels, and samples - used by SDL_LoadWAV_RW
    SDL_AudioSpec file_audio_spec;

    if (nullptr == SDL_LoadWAV_RW(file, 1, &file_audio_spec, &buffer, &length))
    {
        throw std::runtime_error(std::string("can't load wav: ") + path.data());
    }

    std::cout << "audio format for: " << path << '\n'
              << "format: " << get_sound_format_name(file_audio_spec.format)
              << '\n'
              << "sample_size: "
              << get_sound_format_size(file_audio_spec.format) << '\n'
              << "channels: " << static_cast<uint32_t>(file_audio_spec.channels)
              << '\n'
              << "frequency: " << file_audio_spec.freq << '\n'
              << "length: " << length << '\n'
              << "time: "
              << static_cast<double>(length) /
                     (file_audio_spec.channels * file_audio_spec.freq *
                      get_sound_format_size(file_audio_spec.format))
              << "sec" << std::endl;

    if (file_audio_spec.channels != device_audio_spec.channels ||
        file_audio_spec.format != device_audio_spec.format ||
        file_audio_spec.freq != device_audio_spec.freq)
    {
        SDL_AudioCVT cvt;
        SDL_BuildAudioCVT(&cvt, file_audio_spec.format,
                          file_audio_spec.channels, file_audio_spec.freq,
                          device_audio_spec.format, device_audio_spec.channels,
                          device_audio_spec.freq);
        SDL_assert(cvt.needed); // obviously, this one is always needed.
        // read your data into cvt.buf here.
        cvt.len = length;
        // we have to make buffer for inplace conversion
        tmp_buf.reset(new uint8_t[cvt.len * cvt.len_mult]);
        uint8_t* buf = tmp_buf.get();
        std::copy_n(buffer, length, buf);
        cvt.buf = buf;
        if (0 != SDL_ConvertAudio(&cvt))
        {
            std::cout << "failed to convert audio from file: " << path
                      << " to audio device format" << std::endl;
        }
        // cvt.buf has cvt.len_cvt bytes of converted data now.
        SDL_FreeWAV(buffer);
        buffer = tmp_buf.get();
        length = cvt.len_cvt;
    }
}

sound::~sound() {}

sound_buffer_impl::~sound_buffer_impl()
{
    if (!tmp_buf)
    {
        SDL_FreeWAV(buffer);
    }
    buffer = nullptr;
    length = 0;
}

///CLASS SHADER_GL_ES20///
class shader_gl_es20
{
private:
	GLuint vert_shader = 0;
	GLuint frag_shader = 0;
	GLuint program_id = 0;
public:
	shader_gl_es20(
			std::string_view vertex_src, std::string_view fragment_src,
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

	void use() const
	{
		glUseProgram(program_id);
		MY_GL_CHECK();
	}

	void set_uniform(std::string_view uniform_name,
			const texture_gl_es20* texture)
	{
		assert(texture != nullptr);
		const int location =
				glGetUniformLocation(program_id, uniform_name.data());
		MY_GL_CHECK();

		if(location == -1)
		{
			std::cerr << "can't get uniform location from shader\n";
			throw std::runtime_error("can't get uniform location");
		}

		unsigned int texture_unit = 0;
		glActiveTexture_(GL_TEXTURE0 + texture_unit);
		MY_GL_CHECK();
		texture->bind();
		glUniform1i(location, static_cast<int>(0 + texture_unit));
		MY_GL_CHECK();
	}

	void set_uniform(std::string_view uniform_name, const color& c)
	{
		const int location = glGetUniformLocation(program_id, uniform_name.data());
		MY_GL_CHECK();

		if(location == -1)
		{
			std::cerr << "can't get uniform location from shader\n";
			throw std::runtime_error("can't get uniform location");
		}


		float values[4] = { c.get_r(), c.get_g(), c.get_b(), c.get_a() };
		glUniform4fv(location, 1, &values[0]);
		MY_GL_CHECK();
	}

	void set_uniform(std::string_view uniform_name, const matrix& m)
	{
		const int location = glGetUniformLocation(program_id, uniform_name.data());
		MY_GL_CHECK();

		if(location == -1)
		{
			std::cerr << "can't get uniform location from shader\n";
			throw std::runtime_error("can't get uniform location");
		}

		float values[9] = { m.row0.x, m.row0.y, m.row2.x,
							m.row1.x, m.row1.y, m.row2.y,
							0.f, 0.f, 1.f };
		glUniformMatrix3fv(location, 1, GL_FALSE, &values[0]);
		MY_GL_CHECK();
	}
private:
	GLuint compile_shader(GLenum shader_type, std::string_view src)
	{
		GLuint shader_id = glCreateShader(shader_type);
		MY_GL_CHECK();
		std::string_view vertex_shader_src = src;
		const char*      source            = vertex_shader_src.data();
		glShaderSource(shader_id, 1, &source, nullptr);
		MY_GL_CHECK();

		glCompileShader(shader_id);
		MY_GL_CHECK();

		GLint compiled_status = 0;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_status);
		MY_GL_CHECK();
		if (compiled_status == 0)
		{
			GLint info_len = 0;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_len);
			MY_GL_CHECK();
			std::vector<char> info_chars(static_cast<size_t>(info_len));
			glGetShaderInfoLog(shader_id, info_len, NULL, info_chars.data());
			MY_GL_CHECK();
			glDeleteShader(shader_id);
			MY_GL_CHECK();

			std::string shader_type_name =
					shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment";
			std::cerr << "Error compiling shader(vertex)\n"
	                      << vertex_shader_src << "\n"
	                      << info_chars.data();
			return 0;
		}
		return shader_id;
	}

	GLuint link_shader_program(
			const std::vector<std::tuple<GLuint, const GLchar*>>& attributes)
	{
		GLuint shader_id = glCreateProgram();
		MY_GL_CHECK();
		if(0 == shader_id)
		{
			std::cerr << "failed to create gl program";
			throw std::runtime_error("can't link shader");
		}

		glAttachShader(shader_id, vert_shader);
		MY_GL_CHECK();
		glAttachShader(shader_id, frag_shader);
		MY_GL_CHECK();

		for(const auto& attr : attributes)
		{
			GLuint location = std::get<0>(attr);
			const GLchar* name = std::get<1>(attr);
			glBindAttribLocation(shader_id, location, name);
			MY_GL_CHECK();
		}

		glLinkProgram(shader_id);
		MY_GL_CHECK();

		GLint linked_status = 0;
		glGetProgramiv(shader_id, GL_LINK_STATUS, &linked_status);
		MY_GL_CHECK();
		if(linked_status == 0)
		{
			GLint infolen = 0;
			glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &infolen);
			MY_GL_CHECK();
			std::vector<char> infolog(static_cast<size_t>(infolen));
			glGetProgramInfoLog(shader_id, infolen, NULL, infolog.data());
			MY_GL_CHECK();
			std::cerr << "Error linking program:\n" << infolog.data();
			glDeleteProgram(shader_id);
			MY_GL_CHECK();
			return 0;
		}
		return shader_id;
	}
};

///GLOBAL VARIABLES///

std::atomic<bool> engine_already_exist = false;
SDL_Window* window = nullptr;
SDL_GLContext gl_context = nullptr;

shader_gl_es20* shader00 = nullptr;
shader_gl_es20* shader01 = nullptr;
shader_gl_es20* shader02 = nullptr;
shader_gl_es20* shader03 = nullptr;

static void audio_callback(void*, uint8_t*, int);

SDL_AudioDeviceID audio_device;
SDL_AudioSpec audio_device_spec;

std::vector<sound_buffer_impl*> sounds;

///ENGINE IMPL//

std::ostream& operator<<(std::ostream& out, const SDL_version& v)
{
    out << static_cast<int>(v.major) << '.';
    out << static_cast<int>(v.minor) << '.';
    out << static_cast<int>(v.patch);
    return out;
}

engine::engine(std::string_view title)
		: log_(std::cout)
{
	if(engine_already_exist)
	{
		throw std::runtime_error("engine is already initialized");
	}

	std::stringstream serr;

	SDL_version compiled = { 0, 0, 0 };
	SDL_version linked = { 0, 0, 0 };

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	if(SDL_COMPILEDVERSION !=
			SDL_VERSIONNUM(linked.major, linked.minor, linked.patch))
	{
		serr << "warning: SDL2 compiled and linked version mismatch: "
				<< compiled << " " << linked << std::endl;
	}

	const int init_result = SDL_Init(
			SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS |
			SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER);

	if(init_result != 0)
	{
		const char* err_message = SDL_GetError();
		serr << "error: failed call SDL_Init: " << err_message << std::endl;
		throw std::runtime_error(serr.str());
	}

	std::cout << "initialize tiny engine" << std::endl;

	window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 640,
			480, ::SDL_WINDOW_OPENGL);

	if(window == nullptr)
	{
		const char* err_message = SDL_GetError();
		serr << "error: failed call SDL_CreateWindow: " << err_message << std::endl;
		SDL_Quit();
		throw std::runtime_error(serr.str());
	}

	gl_context = SDL_GL_CreateContext(window);
	if(gl_context == nullptr)
	{
		std::string msg("Can't create opengl context: ");
		msg += SDL_GetError();
		serr << msg << std::endl;
		throw std::runtime_error(serr.str());
	}

	int gl_major_ver = 0;
	int result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);
	assert(result == 0);
	int gl_minor_ver = 0;
	result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_major_ver);

	if(gl_major_ver <= 2 && gl_minor_ver < 0)
	{
		serr << "current context opengl version: " << gl_major_ver
				<< gl_minor_ver	<< "need opengl version at least: 2.1\n"
				<< std::flush;
		throw std::runtime_error(serr.str());
	}

	try
	{
		load_gl_func("glCreateShader", glCreateShader);
		load_gl_func("glShaderSource", glShaderSource);
		load_gl_func("glCompileShader", glCompileShader);
		load_gl_func("glGetShaderiv", glGetShaderiv);
		load_gl_func("glGetShaderInfoLog", glGetShaderInfoLog);
		load_gl_func("glDeleteShader", glDeleteShader);
		load_gl_func("glCreateProgram", glCreateProgram);
		load_gl_func("glAttachShader", glAttachShader);
		load_gl_func("glBindAttribLocation", glBindAttribLocation);
		load_gl_func("glLinkProgram", glLinkProgram);
		load_gl_func("glGetProgramiv", glGetProgramiv);
		load_gl_func("glGetProgramInfoLog", glGetProgramInfoLog);
		load_gl_func("glDeleteProgram", glDeleteProgram);
		load_gl_func("glUseProgram", glUseProgram);
		load_gl_func("glVertexAttribPointer", glVertexAttribPointer);
		load_gl_func("glEnableVertexAttribArray",
	                         glEnableVertexAttribArray);
		load_gl_func("glDisableVertexAttribArray",
	                         glDisableVertexAttribArray);
		load_gl_func("glGetUniformLocation", glGetUniformLocation);
		load_gl_func("glUniform1i", glUniform1i);
		load_gl_func("glActiveTexture", glActiveTexture_);
		load_gl_func("glUniform4fv", glUniform4fv);
		load_gl_func("glUniformMatrix3fv", glUniformMatrix3fv);
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		throw;
	}

	shader00 = new shader_gl_es20(
			R"(
			attribute vec2 a_position;
			void main()
			{
				gl_Position = vec4(a_position, 0.0, 1.0);
			})",
			R"(									
			uniform vec4 u_color;
			void main()
			{
				gl_FragColor = u_color;
			}
			)",
			{ { 0, "a_position" } });
	shader00->use();
	shader00->set_uniform("u_color", color(1.f, 0.f, 0.f, 1.f));

	shader01 = new shader_gl_es20(
			R"(
			attribute vec2 a_position;
			attribute vec4 a_color;
			varying vec4 v_color;
			void main()
			{
				v_color = a_color;
				gl_Position = vec4(a_position, 0.0, 1.0);
			})",
			R"(	
			varying vec4 v_color;								
			void main()
			{
				gl_FragColor = v_color;
			}
			)",
			{ { 0, "a_position" }, { 1, "a_color" } });
	shader01->use();

	shader02 = new shader_gl_es20(
			R"(
            attribute vec2 a_position;
            attribute vec2 a_tex_coord;
            attribute vec4 a_color;
            varying vec4 v_color;
            varying vec2 v_tex_coord;
            void main()
            {
            v_tex_coord = a_tex_coord;
            v_color = a_color;
            gl_Position = vec4(a_position, 0.0, 1.0);
            })",
			R"(
			varying vec2 v_tex_coord;
            varying vec4 v_color;
            uniform sampler2D s_texture;
            void main()
            {
                gl_FragColor = texture2D(s_texture, v_tex_coord) * v_color;
            }
            )",
			{ { 0, "a_position" }, { 1, "a_color" }, { 2, "a_tex_coord" } });
	shader02->use();

	shader03 = new shader_gl_es20(
			R"(
			uniform mat3 u_matrix;
			attribute vec2 a_position;
			attribute vec2 a_tex_coord;
			attribute vec4 a_color;
			varying vec4 v_color;
			varying vec2 v_tex_coord;
			void main()
			{
				v_tex_coord = a_tex_coord;
				v_color = a_color;
				vec3 pos = vec3(a_position, 1.0) * u_matrix;
				gl_Position = vec4(pos, 1.0);
			})",
			R"(
			varying vec2 v_tex_coord;
			varying vec4 v_color;
			uniform sampler2D s_texture;							
			void main()
			{
				gl_FragColor = texture2D(s_texture, v_tex_coord) * v_color;
			}
			)",
			{ { 0, "a_position" }, { 1, "a_color" }, { 2, "a_tex_coord" } });
	shader03->use();

	glEnable(GL_BLEND);
	MY_GL_CHECK();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	MY_GL_CHECK();
	glClearColor(0.f, 0.f, 0.f, 0.f);
	MY_GL_CHECK();

	glViewport(0, 0, 640, 480);
	MY_GL_CHECK();

    // initialize audio
    audio_device_spec.freq     = 48000;
    audio_device_spec.format   = AUDIO_S16LSB;
    audio_device_spec.channels = 2;
    audio_device_spec.samples  = 1024; // must be power of 2
    audio_device_spec.callback = audio_callback;
    audio_device_spec.userdata = nullptr;

    const int num_audio_drivers = SDL_GetNumAudioDrivers();
    for (int i = 0; i < num_audio_drivers; ++i)
    {
        std::cout << "audio_driver #:" << i << " " << SDL_GetAudioDriver(i)
                  << '\n';
    }
    std::cout << std::flush;

    // TODO on windows 10 only "directsound" - works for me
    if (std::string_view("Windows") == SDL_GetPlatform())
    {
        const char* selected_audio_driver = SDL_GetAudioDriver(1);
        std::cout << "selected_audio_driver: " << selected_audio_driver
                  << std::endl;

        if (0 != SDL_AudioInit(selected_audio_driver))
        {
            std::cout << "can't initialize SDL audio\n" << std::flush;
        }
    }

    const char* default_audio_device_name = nullptr;

    const int num_audio_devices = SDL_GetNumAudioDevices(SDL_FALSE);
    if (num_audio_devices > 0)
    {
        default_audio_device_name = SDL_GetAudioDeviceName(0, SDL_FALSE);
        for (int i = 0; i < num_audio_devices; ++i)
        {
            std::cout << "audio device #" << i << ": "
                      << SDL_GetAudioDeviceName(i, SDL_FALSE) << '\n';
        }
    }
    std::cout << std::flush;

    audio_device = SDL_OpenAudioDevice(default_audio_device_name, 0,
                                       &audio_device_spec, nullptr,
                                       SDL_AUDIO_ALLOW_ANY_CHANGE);

    if (audio_device == 0)
    {
        std::cerr << "failed open audio device: " << SDL_GetError();
        throw std::runtime_error("audio failed");
    }
    else
    {
        std::cout << "audio device selected: " << default_audio_device_name
                  << '\n'
                  << "freq: " << audio_device_spec.freq << '\n'
                  << "format: "
                  << get_sound_format_name(audio_device_spec.format) << '\n'
                  << "channels: "
                  << static_cast<uint32_t>(audio_device_spec.channels)
                  << '\n'
                  << "samples: " << audio_device_spec.samples << '\n'
                  << std::flush;

        // unpause device
        SDL_PauseAudioDevice(audio_device, SDL_FALSE);
    }
    engine_already_exist = true;
}


float engine::get_time_from_init()
{
    std::uint32_t ms_from_library_initialization = SDL_GetTicks();
    float         seconds = ms_from_library_initialization * 0.001f;
    return seconds;
}

///EVENTS///
struct bind
{
	std::string_view name;
	SDL_Keycode sdl_key;//key
	keys engine_key;//om_key
	bind(std::string_view s, SDL_Keycode sdl_k, keys k)
		: name(s),
		  sdl_key(sdl_k),
		  engine_key(k)
	{
	}
};

const std::array<bind, 8> keys{
	{
		bind{ "up", SDLK_w, keys::up },
		bind{ "left", SDLK_a, keys::left },
		bind{ "right", SDLK_d, keys::right },
		bind{ "down", SDLK_s, keys::down },
		bind{ "button1", SDLK_SPACE, keys::button1 },
		bind{ "button2", SDLK_LSHIFT, keys::button2 },
		bind{ "start", SDLK_LCTRL, keys::start },
		bind{ "select", SDLK_RETURN, keys::select },
	}
};

static bool check_input(const SDL_Event& e, const bind*& result)
{
    using namespace std;

    const auto it = find_if(begin(keys), end(keys), [&](const bind& b) {
        return b.sdl_key == e.key.keysym.sym;
    });

    if (it != end(keys))
    {
        result = &(*it);
        return true;
    }
    return false;
}

bool engine::read_event(event& e)
{
	SDL_Event sdl_event;
	if(SDL_PollEvent(&sdl_event))
	{
		const bind* binding = nullptr;

        if (sdl_event.type == SDL_QUIT)
        {
            e.info      = hardware_data{ true };
            e.timestamp = sdl_event.common.timestamp * 0.001;
            e.type      = event_type::hardware;
            return true;
        }
        else if (sdl_event.type == SDL_KEYDOWN || sdl_event.type == SDL_KEYUP)
        {
            if (check_input(sdl_event, binding))
            {
                bool is_down = sdl_event.type == SDL_KEYDOWN;
                e.info       = input_data{ binding->engine_key, is_down };
                e.timestamp  = sdl_event.common.timestamp * 0.001;
                e.type       = event_type::input_key;
                return true;
            }
        }
	}
	return false;
}

bool engine::is_key_down(const enum keys key)
{
    const auto it = std::find_if(
        begin(keys), end(keys), [&](const bind& b) { return b.engine_key == key; });

    if (it != end(keys))
    {
        const std::uint8_t* state         = SDL_GetKeyboardState(nullptr);
        int                 sdl_scan_code = SDL_GetScancodeFromKey(it->sdl_key);
        return state[sdl_scan_code];
    }
    return false;
}

///AUDIO_CALLBACK///
void audio_callback(void*, uint8_t* stream, int stream_size)
{
    // no sound default
    std::fill_n(stream, stream_size, 0);

    for (sound_buffer_impl* snd : sounds)
    {
        if (snd->is_playing_)
        {
            uint32_t rest         = snd->length - snd->current_index;
            uint8_t* current_buff = &snd->buffer[snd->current_index];

            if (rest <= static_cast<uint32_t>(stream_size))
            {
                // copy rest to buffer
                SDL_MixAudioFormat(stream, current_buff,
                                   audio_device_spec.format, rest,
                                   SDL_MIX_MAXVOLUME);
                snd->current_index += rest;
            }
            else
            {
                SDL_MixAudioFormat(
                    stream, current_buff, audio_device_spec.format,
                    static_cast<uint32_t>(stream_size), SDL_MIX_MAXVOLUME);
                snd->current_index += static_cast<uint32_t>(stream_size);
            }

            if (snd->current_index == snd->length)
            {
                if (snd->is_looped)
                {
                    // start from begining
                    snd->current_index = 0;
                }
                else
                {
                    snd->is_playing_ = false;
                }
            }
        }
    }
}

///TEXTURE///
texture* engine::create_texture(std::string_view path)
{
    return new texture_gl_es20(path);
}

void engine::destroy_texture(texture* t)
{
    delete t;
}

///VBO///
vbo* engine::create_vbo(const tri2* triangles, std::size_t n)
{
    return new vbo_impl(triangles, n);
}

void engine::destroy_vbo(vbo* buffer)
{
    delete buffer;
}

///SOUND///
sound* engine::create_sound(std::string_view path)
{
    SDL_LockAudioDevice(audio_device);
    sound_buffer_impl* s =
        new sound_buffer_impl(path, audio_device, audio_device_spec);
    sounds.push_back(s);
    SDL_UnlockAudioDevice(audio_device);
    return s;
}
void engine::destroy_sound(sound* sound)
{
    delete sound;
}

///RENDERING///
static const std::array<GLenum, 6> primitive_types = {
		{
				GL_LINES,
				GL_LINE_STRIP,
				GL_LINE_LOOP,
				GL_TRIANGLES,
				GL_TRIANGLE_STRIP,
				GL_TRIANGLE_FAN
		}
};

void engine::render(const tri0& t, const color& c)
{
    shader00->use();
    shader00->set_uniform("u_color", c);
    // vertex coordinates
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(v0), &t.v[0].pos.x);
    MY_GL_CHECK();
    glEnableVertexAttribArray(0);
    MY_GL_CHECK();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    MY_GL_CHECK();
}
void engine::render(const tri1& t)
{
    shader01->use();
    // positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(t.v[0]),
                          &t.v[0].pos);
    MY_GL_CHECK();
    glEnableVertexAttribArray(0);
    MY_GL_CHECK();
    // colors
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(t.v[0]),
                          &t.v[0].c);
    MY_GL_CHECK();
    glEnableVertexAttribArray(1);
    MY_GL_CHECK();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    MY_GL_CHECK();

    glDisableVertexAttribArray(1);
    MY_GL_CHECK();
}
void engine::render(const tri2& t, texture* tex)
{
    shader02->use();
    texture_gl_es20* texture = static_cast<texture_gl_es20*>(tex);
    texture->bind();
    shader02->set_uniform("s_texture", texture);
    // positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(t.v[0]),
                          &t.v[0].pos);
    MY_GL_CHECK();
    glEnableVertexAttribArray(0);
    MY_GL_CHECK();
    // colors
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(t.v[0]),
                          &t.v[0].c);
    MY_GL_CHECK();
    glEnableVertexAttribArray(1);
    MY_GL_CHECK();

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(t.v[0]), &t.v[0].uv);
    MY_GL_CHECK();
    glEnableVertexAttribArray(2);
    MY_GL_CHECK();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    MY_GL_CHECK();

    glDisableVertexAttribArray(1);
    MY_GL_CHECK();
    glDisableVertexAttribArray(2);
    MY_GL_CHECK();
}
void engine::render(const tri2& t, texture* tex, const matrix& m)
{
    shader03->use();
    texture_gl_es20* texture = static_cast<texture_gl_es20*>(tex);
    texture->bind();
    shader03->set_uniform("s_texture", texture);
    shader03->set_uniform("u_matrix", m);
    // positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(t.v[0]),
                          &t.v[0].pos);
    MY_GL_CHECK();
    glEnableVertexAttribArray(0);
    MY_GL_CHECK();
    // colors
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(t.v[0]),
                          &t.v[0].c);
    MY_GL_CHECK();
    glEnableVertexAttribArray(1);
    MY_GL_CHECK();

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(t.v[0]), &t.v[0].uv);
    MY_GL_CHECK();
    glEnableVertexAttribArray(2);
    MY_GL_CHECK();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    MY_GL_CHECK();

    glDisableVertexAttribArray(1);
    MY_GL_CHECK();
    glDisableVertexAttribArray(2);
    MY_GL_CHECK();
}

void engine::render(const vbo& buff, texture* tex, const matrix& m)
{
    shader03->use();
    texture_gl_es20* texture = static_cast<texture_gl_es20*>(tex);
    texture->bind();
    shader03->set_uniform("s_texture", texture);
    shader03->set_uniform("u_matrix", m);

    const v2* t = buff.data();
    // positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(v2), &t->pos);
    MY_GL_CHECK();
    glEnableVertexAttribArray(0);
    MY_GL_CHECK();
    // colors
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(v2), &t->c);
    MY_GL_CHECK();
    glEnableVertexAttribArray(1);
    MY_GL_CHECK();

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(v2), &t->uv);
    MY_GL_CHECK();
    glEnableVertexAttribArray(2);
    MY_GL_CHECK();

    GLsizei num_of_vertexes = static_cast<GLsizei>(buff.size());
    glDrawArrays(GL_TRIANGLES, 0, num_of_vertexes);
    MY_GL_CHECK();

    glDisableVertexAttribArray(1);
    MY_GL_CHECK();
    glDisableVertexAttribArray(2);
    MY_GL_CHECK();
}

void engine::swap_buffers()
{
    SDL_GL_SwapWindow(window);

    glClear(GL_COLOR_BUFFER_BIT);
    MY_GL_CHECK();
}

void engine::exit(int return_code)
{
    std::exit(return_code);
}

engine::~engine()
{
    if (engine_already_exist)
    {
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        engine_already_exist = false;
    }
}

