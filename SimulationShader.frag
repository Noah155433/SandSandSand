#version 330 core

uniform sampler2D texture1;

out vec4 FragColor;

void main()
{

	ivec2 pos = ivec2(gl_FragCoord.xy);

	int width = textureSize(texture1, 0).x;
	int height = textureSize(texture1, 0).y;

	float cell = texelFetch(texture1, pos, 0).r;
	float newCell = 0.0;

	ivec2 abovePos = pos + ivec2(0.0, 1.0);
	float aboveCell = texelFetch(texture1, abovePos, 0).r;

	ivec2 belowPos = pos - ivec2(0.0, 1.0);
	float belowCell = texelFetch(texture1, belowPos, 0).r;
	if(belowPos.y < 0)
	{
		belowCell = 1.0;
	}

	if(belowCell > 0.0 && cell > 0.0)
	{
		newCell = 1.0;
	}

	if(aboveCell > 0.0 && cell < 0.1)
	{
		newCell = 1.0;
	}

	FragColor = vec4(newCell, newCell, 0.0, 1.0);
}
