#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
// we have to load all extension GL function pointers
// dynamically from OpenGL library
// so first declare function pointers for all we need
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

///for check standard openGL errors, using function glGetError()
#define OM_GL_CHECK()                                                             \
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

void load()
{
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
}
