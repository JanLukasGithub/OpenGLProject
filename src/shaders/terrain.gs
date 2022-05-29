#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 g_normal;
out vec3 g_position;

uniform mat4 u_viewProj;
uniform mat4 u_view;
uniform mat4 u_invView;

vec3 getNormal() {
   vec3 a = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   return cross(a, b);
}

void main() {
    g_normal = normalize(mat3(u_invView) * getNormal());

    g_position = vec3(u_view * gl_in[0].gl_Position);
    gl_Position = u_viewProj * gl_in[0].gl_Position;
    EmitVertex();
    g_position = vec3(u_view * gl_in[1].gl_Position);
    gl_Position = u_viewProj * gl_in[1].gl_Position;
    EmitVertex();
    g_position = vec3(u_view * gl_in[2].gl_Position);
    gl_Position = u_viewProj * gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}