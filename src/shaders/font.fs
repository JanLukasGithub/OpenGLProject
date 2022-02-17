#version 120

varying vec2 v_texture_coords;

uniform Sampler2D u_texture;

void main() {
    gl_FragColor = vec4(texture2D(u_texture, v_texture_coords).a);
}