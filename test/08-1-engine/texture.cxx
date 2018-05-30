#include "texture.hxx"

#include <vector>

#include "picopng.hxx"

texture_gl_es20::texture_gl_es20(std::string_view path)
    : file_path(path)
{
    std::vector<unsigned char> png_file_in_memory;
    std::ifstream              ifs(path.data(), std::ios_base::binary);
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

    const om::png_image img = om::decode_png_file_from_memory(
    		png_file_in_memory, om::convert_color::to_rgba32,
			om::origin_point::bottom_left);

    // if there's an error, display it
    if (img.error != 0)
    {
        std::cerr << "error: " << img.error << std::endl;
        throw std::runtime_error("can't load texture");
    }

    glGenTextures(1, &tex_handl);
    GL_CHECK();
    glBindTexture(GL_TEXTURE_2D, tex_handl);
    GL_CHECK();

    GLint   mipmap_level = 0;
    GLint   border       = 0;
    GLsizei width        = static_cast<GLsizei>(img.width);
    GLsizei height       = static_cast<GLsizei>(img.height);
    glTexImage2D(GL_TEXTURE_2D, mipmap_level, GL_RGBA, width, height, border,
                 GL_RGBA, GL_UNSIGNED_BYTE, &img.raw_image[0]);
    GL_CHECK();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GL_CHECK();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GL_CHECK();
}

texture_gl_es20::~texture_gl_es20()
{
    glDeleteTextures(1, &tex_handl);
    GL_CHECK();
}
