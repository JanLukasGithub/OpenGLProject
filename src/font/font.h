#ifndef FONT_H_
#define FONT_H_

#include <fstream>
#include <GL/glew.h>
#include <vector>

#include "../../lib/stb_truetype.h"

#include "../utils.h"
#include "../shader.h"

constexpr int FIRST_CHAR = 32;
constexpr int NUM_CHARS = 96;

constexpr int PH_PW = 512;

struct FontVertex {
    glm::vec2 position;
    glm::vec2 texture_coords;
};

struct Font {
    Font(const char* filename, Shader* fontShader);
    ~Font();

    void drawString(float x, float y, const char* text);

private:
    stbtt_bakedchar m_cdata[NUM_CHARS];
    GLuint m_fontTexture;
    GLuint m_fontVao;
    GLuint m_fontVertexBufferId;
    FontVertex* m_fontVertexBufferData = nullptr;
    uint32 m_fontVertexBufferCapacity;

    // Uniform location
    int m_textureUniformLocation{ 0 };
};

#endif