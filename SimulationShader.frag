#version 330 core

uniform sampler2D texture1;

out vec4 FragColor;

void main()
{

	ivec2 pos = ivec2(gl_FragCoord.xy);
	float cell = texelFetch(texture1, ivec2(pos.x, pos.y + 1), 0).r;
	FragColor = vec4(cell, cell, 0.0, 1.0);
}