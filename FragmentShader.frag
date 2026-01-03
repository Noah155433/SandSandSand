#version 330 core

uniform usampler2D texture1;
uniform sampler2D texture2;
uniform vec2 screenSize;

out vec4 FragColor;

vec4 mapIdToColor(uint id)
{
    if (id == 0u) return vec4(0.0, 0.0, 0.0, 1.0);
    if (id == 1u) return vec4(0.92, 0.76, 0.20, 1.0);
    if (id == 2u) return vec4(0.86, 0.55, 0.20, 1.0);
    if (id == 3u) return vec4(0.18, 0.50, 0.86, 1.0);
    if (id == 4u) return vec4(0.24, 0.85, 0.86, 1.0);
    if (id == 5u) return vec4(0.67, 0.64, 0.64, 1.0);
    if (id == 6u) return vec4(0.48, 0.45, 0.45, 1.0);
    return vec4(1.0);
}

void main()
{
    ivec2 coord = ivec2(gl_FragCoord.xy);
    uint id = texelFetch(texture1, coord, 0).r;
    FragColor = mapIdToColor(id);
}