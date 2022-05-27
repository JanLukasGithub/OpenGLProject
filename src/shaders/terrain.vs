#version 450

layout(location = 0) in float a_height;

// Only need x size for division/modulo, no bounds checking using y size
uniform int u_size;
// Offset needs both coords
uniform ivec2 u_offset;

uniform mat4 u_modelViewProj;

const int DISTANCE_BETWEEN_POINTS = 8;

void main() {
    // Using gl_VertexID we can get the info which vertex is rendered, so we don't have to store that manually which would take up a lot of memory
    gl_Position = u_modelViewProj * vec4((u_offset.x + (gl_VertexID % u_size)) * DISTANCE_BETWEEN_POINTS, a_height,
                                        (u_offset.y + (gl_VertexID / u_size)) * DISTANCE_BETWEEN_POINTS, 1.0);
}