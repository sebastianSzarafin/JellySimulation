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

struct Tess
{
    int u;
    int v;
};

uniform Tess u_Tess;

void main()
{
    // Pass through control points
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    // Inner tessellation levels
    gl_TessLevelInner[0] = u_Tess.u;
    gl_TessLevelInner[1] = u_Tess.v;

    // Outer tessellation levels
    gl_TessLevelOuter[0] = u_Tess.v;
    gl_TessLevelOuter[1] = u_Tess.u;
    gl_TessLevelOuter[2] = u_Tess.v;
    gl_TessLevelOuter[3] = u_Tess.u;
}

#define type tess_evaluation
#version 450

layout (quads, equal_spacing, ccw) in;

uniform mat4 u_M;
uniform mat4 u_VP;

out vec3 t_FragPosition;
out vec3 t_FragNormal;

float B(int i, float t);
float dB(int i, float t);

void main()
{
    vec3 p = vec3(0.0);
    vec3 dU = vec3(0.0);
    vec3 dV = vec3(0.0);

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            float B_u = B(i, u);
            float B_v = B(j, v);
            float dB_u = dB(i, u);
            float dB_v = dB(j, v);

            vec3 P = gl_in[i * 4 + j].gl_Position.xyz;
            p += P * B_u * B_v;

            dU += P * dB_u * B_v;
            dV += P * B_u * dB_v;
        }
    }

    t_FragPosition = vec3(u_M * vec4(p, 1.0));
    t_FragNormal = normalize(cross(dU, dV));

    // Transformacja punktu
    gl_Position = u_VP * u_M * vec4(p, 1.0);
}

float B(int i, float t)
{
    if (i == 0) return (1 - t) * (1 - t) * (1 - t);
    if (i == 1) return 3.0 * t * (1 - t) * (1 - t);
    if (i == 2) return 3.0 * t * t * (1 - t);
    if (i == 3) return t * t * t;
    return 0.0;
}

float dB(int i, float t)
{
    if (i == 0) return -3.0 * (1.0 - t) * (1.0 - t);
    if (i == 1) return 9.0 * t * t - 12 * t + 3.0;
    if (i == 2) return -9.0 * t * t + 6.0 * t;
    if (i == 3) return 3.0 * t * t;
    return 0.0;
}

#define type fragment
#version 450

layout (location = 0) out vec4 color;

const float AmbientStrength = 0.2;
const float SpecularStrength = 0.5;
const float Shininess = 32;

in vec3 t_FragPosition;
in vec3 t_FragNormal;

struct Light
{
    vec3 pos;
    vec3 color;
};

uniform vec3 u_CameraPos;
uniform Light u_Light;
uniform vec3 u_FragColor;

void main()
{
    // Ambient
    vec3 ambient = AmbientStrength * u_Light.color;

    // Diffuse
    vec3 lightDir = normalize(u_Light.pos - t_FragPosition);
    float brightness = max(dot(t_FragNormal, lightDir), 0.0);
    vec3 diffuse = brightness * u_Light.color;

    // Specular
    vec3 viewDir = normalize(u_CameraPos - t_FragPosition);
    vec3 reflectDir = reflect(-lightDir, t_FragNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 specular = spec * u_Light.color;

    // color = vec4(abs(t_FragNormal), 1.0);
    color = vec4((ambient + diffuse + specular) * u_FragColor, 1.0);
}