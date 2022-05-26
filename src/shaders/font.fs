#version 450

in vec2 v_textureCoords;

layout(location = 0) out vec4 f_color;

uniform sampler2D u_texture;

void main() {
    f_color = vec4(texture(u_texture, v_textureCoords).a);
}