#include "gui.h"

void Gui::drawRectangle(float32 x, float32 y, float32 w, float32 h, int32 color) noexcept {
    // Don't use texture
    glUniform1i(glGetUniformLocation(Renderer::activeRenderer->getShader2d()->getShaderId(), "u_hasTexture"), 0);
    // Pass color to uniform
    float32 red = ((0xFF000000 & color) >> 24) / 255.0f;
    float32 green = ((0x00FF0000 & color) >> 16) / 255.0f;
    float32 blue = ((0x0000FF00 & color) >> 8) / 255.0f;
    float32 alpha = (0x000000FF & color) / 255.0f;
    glUniform4f(glGetUniformLocation(Renderer::activeRenderer->getShader2d()->getShaderId(), "u_color"), red, green, blue, alpha);

    // Positions of vertices
    float32 buffer[] = {
        x, y,
        x + w, y,
        x, y + h,
        x + w, y + h
    };

    // Vertex array
    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    // Vertex buffer
    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    // Copy data to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer[0]) * 8, buffer, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Delete buffer and array
    glDeleteBuffers(1, &vertexBufferId);
    glDeleteVertexArrays(1, &vertexArray);
}
