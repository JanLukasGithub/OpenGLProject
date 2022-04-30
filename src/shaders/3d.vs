#version 330 core

// a_ means attribute
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec2 a_textureCoords;

// v_ means vertex
out vec3 v_position;
out vec2 v_textureCoords;
out mat3 v_tbn;

uniform mat4 u_modelMat;
uniform mat4 u_modelViewProj;
uniform mat4 u_modelView;
uniform mat4 u_invModelView;

uniform bool u_hasNormalMap;

void main() {
    vec4 worldSpacePosition = u_modelMat * vec4(a_position, 1.0f);

    gl_Position = u_modelViewProj * worldSpacePosition;
    
    v_position = vec3(u_modelView * worldSpacePosition);

    vec3 t = normalize(mat3(u_invModelView) * a_tangent);
    vec3 n = normalize(mat3(u_invModelView) * a_normal);
    t = normalize(t - dot(t, n) * n); // Reorthogonalize with Gram-Schmidt process
    vec3 b = normalize(mat3(u_invModelView) * cross(n, t));
    v_tbn = transpose(mat3(t, b, n)) * int(u_hasNormalMap) + mat3(1.0) * int(!u_hasNormalMap);
    
    v_textureCoords = a_textureCoords;
}