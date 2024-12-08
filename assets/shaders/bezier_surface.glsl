#define type vertex
#version 450

layout (location = 0) in vec3 a_Position;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
}

#define type tess_control
#version 450

layout (vertices = 16) out;

uniform int u_Tess_u;
uniform int u_Tess_v;

void main()
{
    // Pass through control points
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    // Inner tessellation levels
    gl_TessLevelInner[0] = u_Tess_u;
    gl_TessLevelInner[1] = u_Tess_v;

    // Outer tessellation levels
    gl_TessLevelOuter[0] = u_Tess_v;
    gl_TessLevelOuter[1] = u_Tess_u;
    gl_TessLevelOuter[2] = u_Tess_v;
    gl_TessLevelOuter[3] = u_Tess_u;
}

#define type tess_evaluation
#version 450

layout (quads, equal_spacing, ccw) in;

uniform mat4 u_MVP;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p10 = gl_in[1].gl_Position;
    vec4 p20 = gl_in[2].gl_Position;
    vec4 p30 = gl_in[3].gl_Position;
    vec4 p01 = gl_in[4].gl_Position;
    vec4 p11 = gl_in[5].gl_Position;
    vec4 p21 = gl_in[6].gl_Position;
    vec4 p31 = gl_in[7].gl_Position;
    vec4 p02 = gl_in[8].gl_Position;
    vec4 p12 = gl_in[9].gl_Position;
    vec4 p22 = gl_in[10].gl_Position;
    vec4 p32 = gl_in[11].gl_Position;
    vec4 p03 = gl_in[12].gl_Position;
    vec4 p13 = gl_in[13].gl_Position;
    vec4 p23 = gl_in[14].gl_Position;
    vec4 p33 = gl_in[15].gl_Position;

    float bu0 = (1. - u) * (1. - u) * (1. - u);
    float bu1 = 3. * u * (1. - u) * (1. - u);
    float bu2 = 3. * u * u * (1. - u);
    float bu3 = u * u * u;

    float bv0 = (1. - v) * (1. - v) * (1. - v);
    float bv1 = 3. * v * (1. - v) * (1. - v);
    float bv2 = 3. * v * v * (1. - v);
    float bv3 = v * v * v;

    vec4 p = bv0 * (bu0 * p00 + bu1 * p01 + bu2 * p02 + bu3 * p03)
    + bv1 * (bu0 * p10 + bu1 * p11 + bu2 * p12 + bu3 * p13)
    + bv2 * (bu0 * p20 + bu1 * p21 + bu2 * p22 + bu3 * p23)
    + bv3 * (bu0 * p30 + bu1 * p31 + bu2 * p32 + bu3 * p33);

    gl_Position = u_MVP * p;
}

#define type fragment
#version 450

layout (location = 0) out vec4 color;

uniform vec3 u_Color;

void main()
{
    color = vec4(u_Color, 1.0);
}