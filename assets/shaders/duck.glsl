#define type vertex
#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_MVP;
uniform vec3 u_ControlPoints[64];

out vec2 v_TexCoord;

float B(int i, float t);
vec3 deform(vec3 uvw);

void main()
{
    gl_Position = u_MVP * vec4(deform(a_Position), 1.0);

    v_TexCoord = a_TexCoord;
}

float B(int i, float t)
{
    if (i == 0) return (1 - t) * (1 - t) * (1 - t);
    if (i == 1) return 3.0 * t * (1 - t) * (1 - t);
    if (i == 2) return 3.0 * t * t * (1 - t);
    if (i == 3) return t * t * t;
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

#define type fragment
#version 450

layout (location = 0) out vec4 color;

uniform sampler2D u_Texture;

in vec2 v_TexCoord;

void main()
{
    color = texture(u_Texture, v_TexCoord);
}