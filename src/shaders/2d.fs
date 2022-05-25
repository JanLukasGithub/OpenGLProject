#version 450

in vec2 v_textureCoords;

layout(location = 0) out vec4 f_color;

uniform sampler2D u_texture;
uniform bool u_hasTexture;
uniform vec4 u_color;

void main() {
    f_color = int(u_hasTexture) * vec4(texture(u_texture, v_textureCoords)) + int(!u_hasTexture) * u_color;
}