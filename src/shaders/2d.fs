#version 330 core

in vec2 v_textureCoords;

uniform sampler2D u_texture;
uniform bool u_hasTexture;
uniform vec4 u_color;

void main() {
    if (u_hasTexture)
        gl_FragColor = vec4(texture2D(u_texture, v_textureCoords));
    else
        gl_FragColor = u_color;
}