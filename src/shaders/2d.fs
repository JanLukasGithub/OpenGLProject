#version 330 core

in vec2 v_texture_coords;

uniform sampler2D u_texture;
uniform bool u_onlyAlpha;

void main() {
    if (u_onlyAlpha)
        gl_FragColor = vec4(texture2D(u_texture, v_texture_coords).a);
    else
        gl_FragColor = vec4(texture2D(u_texture, v_texture_coords));
}