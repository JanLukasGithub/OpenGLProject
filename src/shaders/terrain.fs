#version 450

layout(location = 0) out vec4 f_color;

in vec3 g_normal;

void main() {
    f_color = vec4(0, 1, 0, 1);
}