// Only this file implements stb_truetype
#define STB_TRUETYPE_IMPLEMENTATION
#include "font.h"

Font::~Font() {
    if (m_fontVertexBufferData)
        delete[] m_fontVertexBufferData;
}

void Font::initFont(const char* filename) {
    // Open the file:
    std::streampos fileSize;
    std::ifstream file(filename, std::ios::binary);

    // Get its size
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Buffers to store data in
    std::vector<byte>* ttfBuffer = new std::vector<byte>(fileSize);
    std::vector<byte>* tmpBitmap = new std::vector<byte>();

    // Read file to the buffer
    file.read((char*) &(*ttfBuffer)[0], fileSize);
    stbtt_BakeFontBitmap(&(*ttfBuffer)[0], 0, 32.0f, &(*tmpBitmap)[0], PH_PW, PH_PW, FIRST_CHAR, NUM_CHARS, m_cdata);

    // Generate OpenGL texture
    glGenTextures(1, &m_fontTexture);
    glBindTexture(GL_TEXTURE_2D, m_fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, PH_PW, PH_PW, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tmpBitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Data is now stored in VRAM, delete RAM version
    delete tmpBitmap;
    delete ttfBuffer;

    // Generate vertex array
    glGenVertexArrays(1, &m_fontVao);
    glBindVertexArray(m_fontVao);
    // Generate vertex buffer on GPU
    glGenBuffers(1, &m_fontVertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_fontVertexBufferId);
    // Generate vertex buffer on CPU
    m_fontVertexBufferCapacity = 20;
    m_fontVertexBufferData = new FontVertex[m_fontVertexBufferCapacity * 6];
    // Setup vertex buffer on GPU with right size
    glBufferData(GL_ARRAY_BUFFER, sizeof(FontVertex) * 6 * m_fontVertexBufferCapacity, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(FontVertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FontVertex), (const void*) offsetof(FontVertex, texture_coords));
    // Unbind vertex array
    glBindVertexArray(0);
}

void Font::drawString(float x, float y, const char* text, Shader* fontShader) {
    glBindVertexArray(m_fontVao);

    uint32 len = strlen(text);

    // If the existing buffer is to small
    if (m_fontVertexBufferCapacity < len) {
        // Use the length of the text as new buffer size
        m_fontVertexBufferCapacity = len;
        // Update both GPU buffer and CPU buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(FontVertex) * 6 * m_fontVertexBufferCapacity, nullptr, GL_DYNAMIC_DRAW);
        delete[] m_fontVertexBufferData;
        m_fontVertexBufferData = new FontVertex[m_fontVertexBufferCapacity * 6];
    }

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fontTexture);
    glUniform1i(glGetUniformLocation(fontShader->getShaderId(), "u_texture"), 0);

    // Pointer to data
    FontVertex* vData = m_fontVertexBufferData;
    uint32 numVertices = 0;

    // Draw chars
    while (*text) {
        // Check if char is printable
        if (*text >= FIRST_CHAR && *text < FIRST_CHAR + NUM_CHARS) {
            // Get the quad
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(m_cdata, PH_PW, PH_PW, *text-FIRST_CHAR, &x, &y, &q, 1);

            vData[0].position = glm::vec2(q.x0, q.y0); vData[0].texture_coords = glm::vec2(q.s0, q.t0);
            vData[1].position = glm::vec2(q.x1, q.y0); vData[1].texture_coords = glm::vec2(q.s1, q.t0);
            vData[2].position = glm::vec2(q.x1, q.y1); vData[2].texture_coords = glm::vec2(q.s1, q.t1);
            vData[3].position = glm::vec2(q.x0, q.y1); vData[3].texture_coords = glm::vec2(q.s0, q.t1);
            vData[4].position = glm::vec2(q.x0, q.y0); vData[4].texture_coords = glm::vec2(q.s0, q.t0);
            vData[5].position = glm::vec2(q.x1, q.y1); vData[5].texture_coords = glm::vec2(q.s1, q.t1);

            vData += 6;
            numVertices += 6;
        }
        
        ++text;
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FontVertex) * numVertices, m_fontVertexBufferData);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
}