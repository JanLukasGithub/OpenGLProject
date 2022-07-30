#version 450

// a_ means attribute
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec2 a_textureCoords;

// v_ means vertex
out vec3 v_view_space_position;
out vec2 v_textureCoords;
out mat3 v_tbn;

layout(std430, binding = 0) buffer matricesBlock {
    mat4 matrices[];
} b_modelMats;

uniform mat4 u_view_proj;
uniform mat4 u_view;
uniform mat4 u_inv_view;

void main() {
    vec4 world_space_position = b_modelMats.matrices[gl_InstanceID] * vec4(a_position, 1.0f);

    gl_Position = u_view_proj * world_space_position;
    
    v_view_space_position = vec3(u_view * world_space_position);

    vec3 t = normalize(vec3(u_inv_view * vec4(a_tangent, 1.0f)));
    vec3 n = normalize(vec3(u_inv_view * vec4(a_normal, 1.0f)));
    vec3 b = normalize(mat3(u_inv_view) * cross(n, t));
    v_tbn = transpose(mat3(t, b, n));
    
    v_textureCoords = a_textureCoords;
}