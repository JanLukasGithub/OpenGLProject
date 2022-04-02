#version 330 core

in vec2 v_textureCoords;

uniform sampler2D u_texture;
uniform bool u_hasTexture;
uniform vec4 u_color;

void main() {
    gl_FragColor = int(u_hasTexture) * vec4(texture2D(u_texture, v_textureCoords)) + int(!u_hasTexture) * u_color;
}