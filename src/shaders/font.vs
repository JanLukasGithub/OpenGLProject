#version 120

attribute vec2 a_position;
attribute vec2 a_texture_coords;

varying vec2 v_texture_coords;

uniform mat4 u_modelViewProj;

void main() {
    glPosition = u_modelViewProj * vec4(a_position, 0.5f, 1.0f);

    v_texture_coords = a_texture_coords;
}