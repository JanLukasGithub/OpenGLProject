#version 330 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texture_coords;

out vec2 v_texture_coords;

uniform mat4 u_modelViewProj;

void main() {
    gl_Position = u_modelViewProj * vec4(a_position, 0.5f, 1.0f);

    v_texture_coords = a_texture_coords;
}