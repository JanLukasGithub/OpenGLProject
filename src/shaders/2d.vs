#version 330 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_textureCoords;

out vec2 v_textureCoords;

uniform mat4 u_modelViewProj;

void main() {
    gl_Position = u_modelViewProj * vec4(a_position, 0.5f, 1.0f);

    a_textureCoords = a_textureCoords;
}