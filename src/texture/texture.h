#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <iostream>

#include <GL/glew.h>

#include "../../lib/stb_image.h"

struct Texture {
public:
    Texture();
    Texture(std::string filename);
    Texture(const Texture& tex) = delete;
    Texture(Texture&& tex);

    friend bool operator==(const Texture& first, const Texture& second);
    friend bool operator==(const Texture& tex, const std::string filename);

    /** Returns true if and only if this actually has an associated texture stored in VRAM */
    bool has_texture() const noexcept;

    /** target like in glBindTexture(). texture like in glActiveTexture() */
    void bind(GLenum target, GLenum texture) const noexcept;

    std::string m_filename{ "" };
    GLuint m_texture_id{ 0 };
};

#endif