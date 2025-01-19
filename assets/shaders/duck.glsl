#define type vertex
#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_M;
uniform mat4 u_VP;
uniform vec3 u_ControlPoints[64];

out vec2 v_TexCoord;
out vec3 v_FragPosition;
out vec3 v_FragNormal;


float B(int i, float t);
float dB(int i, float t);
vec3 deform(vec3 uvw);
vec3 deformNormal(vec3 uvw, vec3 normal, mat3 model);

void main()
{
    vec3 uvw = vec3(u_M * vec4(a_Position, 1.0));
    vec3 deformedUvw = deform(uvw);
    gl_Position = u_VP * vec4(deformedUvw, 1.0);

    v_TexCoord = a_TexCoord;
    v_FragPosition = deformedUvw;
    v_FragNormal = deformNormal(uvw, a_Normal, mat3(u_M));
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

vec3 deform(vec3 uvw)
{
    vec3 result = vec3(0.0);
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                float weight = B(i, uvw.x) * B(j, uvw.y) * B(k, uvw.z);
                int idx = i * 16 + j * 4 + k;
                result += u_ControlPoints[idx] * weight;
            }
        }
    }
    return result;
}

mat3 adjoint(mat3 m)
{
    return mat3(
        m[1][1] * m[2][2] - m[1][2] * m[2][1], // Cofactor of m[0][0]
        m[0][2] * m[2][1] - m[0][1] * m[2][2], // Cofactor of m[0][1]
        m[0][1] * m[1][2] - m[0][2] * m[1][1], // Cofactor of m[0][2]

        m[1][2] * m[2][0] - m[1][0] * m[2][2], // Cofactor of m[1][0]
        m[0][0] * m[2][2] - m[0][2] * m[2][0], // Cofactor of m[1][1]
        m[0][2] * m[1][0] - m[0][0] * m[1][2], // Cofactor of m[1][2]

        m[1][0] * m[2][1] - m[1][1] * m[2][0], // Cofactor of m[2][0]
        m[0][1] * m[2][0] - m[0][0] * m[2][1], // Cofactor of m[2][1]
        m[0][0] * m[1][1] - m[0][1] * m[1][0]  // Cofactor of m[2][2]
    );
}

vec3 deformNormal(vec3 uvw, vec3 normal, mat3 model)
{
    vec3 F_u = vec3(0.0);
    vec3 F_v = vec3(0.0);
    vec3 F_w = vec3(0.0);

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                int idx = (i + 1) * 16 + j * 4 + k;
                vec3 PP = u_ControlPoints[idx];

                idx = i * 16 + j * 4 + k;
                vec3 PN = u_ControlPoints[idx];

                F_u += (PP - PN) * B(i, uvw.x) * B(j, uvw.y) * B(k, uvw.z);
            }
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                int idx = i * 16 + (j + 1) * 4 + k;
                vec3 PP = u_ControlPoints[idx];

                idx = i * 16 + j * 4 + k;
                vec3 PN = u_ControlPoints[idx];

                F_v += (PP - PN) * B(i, uvw.x) * B(j, uvw.y) * B(k, uvw.z);
            }
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                int idx = i * 16 + j * 4 + (k + 1);
                vec3 PP = u_ControlPoints[idx];

                idx = i * 16 + j * 4 + k;
                vec3 PN = u_ControlPoints[idx];

                F_w += (PP - PN) * B(i, uvw.x) * B(j, uvw.y) * B(k, uvw.z);
            }
        }
    }

    mat3 J = mat3(F_u, F_v, F_w);
    vec3 deformedNormal = transpose(inverse(model) * adjoint(J)) * normal;
    return normalize(deformedNormal);
}

#define type fragment
#version 450

layout (location = 0) out vec4 color;

const float AmbientStrength = 0.2;
const float SpecularStrength = 0.5;
const float Shininess = 32;

in vec3 v_FragPosition;
in vec3 v_FragNormal;
in vec2 v_TexCoord;

struct Light
{
    vec3 pos;
    vec3 color;
};

uniform vec3 u_CameraPos;
uniform Light u_Light;
uniform sampler2D u_Texture;

void main()
{
    // Ambient
    vec3 ambient = AmbientStrength * u_Light.color;

    // Diffuse
    vec3 lightDir = normalize(u_Light.pos - v_FragPosition);
    float brightness = max(dot(v_FragNormal, lightDir), 0.0);
    vec3 diffuse = brightness * u_Light.color;

    // Specular
    vec3 viewDir = normalize(u_CameraPos - v_FragPosition);
    vec3 reflectDir = reflect(-lightDir, v_FragNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 specular = spec * u_Light.color;

    color = vec4(ambient + diffuse + specular, 1.0) * texture(u_Texture, v_TexCoord);
}