#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

uniform vec2 mousePos;
uniform bool leftButtonPressed;
uniform vec3 spawnColor;

void newState(vec3 value)
{
	FragColor = vec4(value, 1.0);
	return;
}
	
vec3 getPixelValue(ivec2 offset, ivec2 pos)
{

	ivec2 size = textureSize(texture1, 0);
	ivec2 p = pos + offset;

	if (p.y < 0)
        return vec3(1.0, 0.0, 0.0);

	if (p.x < size.x / 5 || p.x > size.x - 1)
	{
		if (offset.y > 0)
		{
			return vec3(0.0);
		}
		return vec3(1.0, 0.0, 0.0);
	}

	return texelFetch(texture1, p, 0).rgb;
}

void main()
{

	ivec2 pos = ivec2(gl_FragCoord.xy);

	ivec2 size = textureSize(texture1, 0);


	vec3 cell = texelFetch(texture1, pos, 0).rgb;

	vec3 aboveCell = getPixelValue(ivec2(0.0, 1.0), pos);
	vec3 aboveRightCell = getPixelValue(ivec2(1.0, 1.0), pos);
	vec3 aboveLeftCell = getPixelValue(ivec2(-1.0, 1.0), pos);

	vec3 belowCell = getPixelValue(ivec2(0.0, -1.0), pos);
	vec3 belowRightCell = getPixelValue(ivec2(1.0, -1.0), pos);
	vec3 belowLeftCell = getPixelValue(ivec2(-1.0, -1.0), pos);

	vec3 rightCell = getPixelValue(ivec2(1.0, 0.0), pos);
	vec3 leftCell = getPixelValue(ivec2(-1.0, 0.0), pos);

	if(distance(vec2(pos), mousePos) < 5 && leftButtonPressed && mousePos.x > size.x / 5 + 5)
	{
		newState(spawnColor * texelFetch(texture2, pos, 0).r);
		return;
	}

	if(cell.r > 0.0)
	{
		if(belowCell.r < 1.0)
		{
			newState(vec3(0.0));
			return;
		}
		if((belowRightCell.r < 1.0 || belowLeftCell.r < 1.0) && belowCell.r > 0.0)
		{
			newState(vec3(0.0));
			return;
		}
	}

	if(cell.b > 0.0)
	{
		if(belowCell.r < 1.0 && belowCell.b < 1.0)
		{
			newState(vec3(0.0));
			return;
		}
		if(((belowRightCell.b < 1.0 && belowRightCell.r < 1.0) || (belowLeftCell.b < 1.0 && belowLeftCell.r < 1.0)) && (belowCell.b > 0.0 || belowCell.r > 0.0))
		{
			newState(vec3(0.0));
			return;
		}
		if(aboveCell.r > 0.0)
		{
			newState(aboveCell);
			return;
		}
	}

	if(cell.r < 0.1 && cell.b < 0.1)
	{
		if(aboveCell.r > 0.0 || aboveCell.b > 0.0)
		{
			newState(aboveCell);
			return;
		}
		if((aboveRightCell.r > 0.0 || aboveRightCell.b > 0.0) && (belowCell.r > 0.0 || belowCell.b > 0.0))
		{
			newState(aboveRightCell);
			return;
		}
		if((aboveLeftCell.r > 0.0 || aboveLeftCell.b > 0.0) && (belowCell.r > 0.0 || belowCell.b > 0.0))
		{
			newState(aboveLeftCell);
			return;
		}
	}
	
	FragColor = vec4(cell, 1.0);
}

