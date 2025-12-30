#version 330 core

uniform sampler2D texture1;

out vec4 FragColor;

uniform vec2 mousePos;
uniform bool leftButtonPressed;

void main()
{

	ivec2 pos = ivec2(gl_FragCoord.xy);

	int width = textureSize(texture1, 0).x;
	int height = textureSize(texture1, 0).y;


	float cell = texelFetch(texture1, pos, 0).r;
	float newCell = 0.0;

	ivec2 abovePos = pos + ivec2(0.0, 1.0);
	float aboveCell = texelFetch(texture1, abovePos, 0).r;

	ivec2 aboveRightPos = pos + ivec2(1.0, 1.0);
	float aboveRightCell = texelFetch(texture1, aboveRightPos, 0).r;

	if(aboveRightPos.x > width || aboveRightPos.y > height)
	{
		aboveRightCell = 0.0;
	}

	ivec2 aboveLeftPos = pos + ivec2(-1.0, 1.0);
	float aboveLeftCell = texelFetch(texture1, aboveLeftPos, 0).r;

	if(aboveLeftPos.x < 256 || aboveLeftPos.y > height)
	{
		aboveLeftCell = 0.0;
	}

	ivec2 belowPos = pos - ivec2(0.0, 1.0);
	float belowCell = texelFetch(texture1, belowPos, 0).r;

	if(belowPos.y < 0)
	{
		belowCell = 1.0;
	}

	ivec2 belowLeftPos = pos + ivec2(-1.0, -1.0);
	float belowLeftCell = texelFetch(texture1, belowLeftPos, 0).r;

	if(belowLeftPos.x < 256 || belowLeftPos.y < 0)
	{
		belowLeftCell = 1.0;
	}

	ivec2 belowRightPos = pos + ivec2(1.0, -1.0);
	float belowRightCell = texelFetch(texture1, belowRightPos, 0).r;

	if(belowRightPos.x > width - 1 || belowRightPos.y < 0)
	{
		belowRightCell = 1.0;
	}

	if(cell > 0.0)
	{
		newCell = 1.0;
		if(belowCell < 0.1)
		{
			newCell = 0.0;
		}
		else if(belowLeftCell < 0.1)
		{
			newCell = 0.0;
		}
		else if(belowRightCell < 0.1)
		{
			newCell = 0.0;
		}
	}

	if(aboveCell > 0.0 && cell < 0.1)
	{
		newCell = 1.0;
	}

	if(cell < 0.1)
	{
		if(aboveRightCell > 0.1 && belowCell > 0.1)
		{
			newCell = 1.0;
		}
		else if(aboveLeftCell > 0.1 && belowCell > 0.1)
		{
			newCell = 1.0;
		}
	}

	if(distance(vec2(pos), mousePos) < 5 && leftButtonPressed)
	{
		newCell = 1.0;
	}

	FragColor = vec4(newCell, newCell, 0.0, 1.0);
}
	