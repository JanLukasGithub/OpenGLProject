#version 450

// a_ means attribute
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec2 a_textureCoords;

// v_ means vertex
out vec3 v_position;
out vec2 v_textureCoords;
out mat3 v_tbn;

layout(std430, binding = 0) buffer matricesBlock {
    mat4 matrices[];
} b_modelMats;

uniform mat4 u_modelViewProj;
uniform mat4 u_modelView;
uniform mat4 u_invModelView;

void main() {
    vec4 worldSpacePosition = b_modelMats.matrices[gl_InstanceID] * vec4(a_position, 1.0f);

    gl_Position = u_modelViewProj * worldSpacePosition;
    
    v_position = vec3(u_modelView * worldSpacePosition);

    vec3 t = normalize(mat3(u_invModelView) * a_tangent);
    vec3 n = normalize(mat3(u_invModelView) * a_normal);
    vec3 b = normalize(mat3(u_invModelView) * cross(n, t));
    v_tbn = transpose(mat3(t, b, n));
    
    v_textureCoords = a_textureCoords;
}