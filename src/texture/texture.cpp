#include "texture.h"

Texture::Texture() {}

Texture::Texture(std::string filename) : m_filename{ filename } {
    if (filename == "")
        return;

    int texture_width = 0;
    int texture_height = 0;
    int bytes_per_pixel = 0;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* texture_buffer = stbi_load(filename.c_str(), &texture_width, &texture_height, &bytes_per_pixel, 4);
    if (!texture_buffer) {
        std::cerr << "Couldn't load image at " << filename << "!" << std::endl;
        return;
    }

    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_buffer);

    stbi_image_free(texture_buffer);
}

Texture::Texture(Texture&& tex) : m_filename{ std::move(tex.m_filename) }, m_texture_id{ tex.m_texture_id } {
    tex.m_texture_id = 0;
}

bool operator==(const Texture& first, const Texture& second) {
    return first.m_filename == second.m_filename;
}

bool operator==(const Texture& tex, const std::string filename) {
    return tex.m_filename == filename;
}

bool Texture::has_texture() const noexcept {
    return m_texture_id != 0;
}

void Texture::bind(GLenum target, GLenum texture) const noexcept {
    glActiveTexture(texture);
    glBindTexture(target, m_texture_id);
}
