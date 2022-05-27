#define STB_TRUETYPE_IMPLEMENTATION
#include "font.h"

void Font::initUniforms(Shader* fontShader) {
    Font::textureUniformLocation = glGetUniformLocation(fontShader->getShaderId(), "u_texture");
}

Font::Font(const std::string& filename, Shader* fontShader) {
    std::streampos fileSize;
    std::ifstream file(filename, std::ios::binary);

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<byte> ttfBuffer = std::vector<byte>(fileSize);
    std::vector<byte> tmpBitmap = std::vector<byte>(PH_PW * PH_PW);

    file.read((char*)ttfBuffer.data(), fileSize);

    stbtt_BakeFontBitmap(ttfBuffer.data(), 0, 32.0f, tmpBitmap.data(), PH_PW, PH_PW, FIRST_CHAR, NUM_CHARS, m_cdata);

    glGenTextures(1, &m_fontTexture);
    glBindTexture(GL_TEXTURE_2D, m_fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, PH_PW, PH_PW, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tmpBitmap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &m_fontVao);
    glBindVertexArray(m_fontVao);

    glGenBuffers(1, &m_fontVertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_fontVertexBufferId);

    m_fontVertexBufferCapacity = 20;
    m_fontVertexBufferData = new FontVertex[m_fontVertexBufferCapacity * 6];
    glBufferData(GL_ARRAY_BUFFER, sizeof(FontVertex) * 6 * m_fontVertexBufferCapacity, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(FontVertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FontVertex), (const void*)offsetof(FontVertex, texture_coords));
}

Font::~Font() {
    if (m_fontVertexBufferData)
        delete[] m_fontVertexBufferData;

    glDeleteVertexArrays(1, &m_fontVao);
    glDeleteBuffers(1, &m_fontVertexBufferId);
    glDeleteTextures(1, &m_fontTexture);
}

void Font::drawString(float x, float y, const std::string& text) {
    glBindVertexArray(m_fontVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_fontVertexBufferId);

    if (m_fontVertexBufferCapacity < text.length()) {
        m_fontVertexBufferCapacity = text.length();

        glBufferData(GL_ARRAY_BUFFER, sizeof(FontVertex) * 6 * m_fontVertexBufferCapacity, nullptr, GL_DYNAMIC_DRAW);
        delete[] m_fontVertexBufferData;
        m_fontVertexBufferData = new FontVertex[m_fontVertexBufferCapacity * 6];
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fontTexture);
    glUniform1i(Font::textureUniformLocation, 0);

    FontVertex* vData = m_fontVertexBufferData;
    uint32 numVertices = 0;

    for (int i = 0; i < text.length(); i++) {
        if (text[i] >= FIRST_CHAR && text[i] < FIRST_CHAR + NUM_CHARS) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(m_cdata, PH_PW, PH_PW, text[i] - FIRST_CHAR, &x, &y, &q, 1);

            vData[0].position = glm::vec2(q.x0, q.y0); vData[0].texture_coords = glm::vec2(q.s0, q.t0);
            vData[1].position = glm::vec2(q.x1, q.y0); vData[1].texture_coords = glm::vec2(q.s1, q.t0);
            vData[2].position = glm::vec2(q.x1, q.y1); vData[2].texture_coords = glm::vec2(q.s1, q.t1);
            vData[3].position = glm::vec2(q.x0, q.y1); vData[3].texture_coords = glm::vec2(q.s0, q.t1);
            vData[4].position = glm::vec2(q.x0, q.y0); vData[4].texture_coords = glm::vec2(q.s0, q.t0);
            vData[5].position = glm::vec2(q.x1, q.y1); vData[5].texture_coords = glm::vec2(q.s1, q.t1);

            vData += 6;
            numVertices += 6;
        }
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FontVertex) * numVertices, m_fontVertexBufferData);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
}