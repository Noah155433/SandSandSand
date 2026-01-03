#version 330 core
layout(location = 0) out uvec4 outColor;

uniform usampler2D texture1;
uniform sampler2D texture2;

uniform vec2 mousePos;
uniform bool leftButtonPressed;
uniform int spawnColor;

void newState(int value)
{
	if(value == 0)
	{
		outColor = uvec4(0, 0, 0, 1);
		return;
	}
	if(value == 1)
	{
		outColor = uvec4(1, 0, 0, 1);
		return;
	}
	if(value == 2)
	{
		outColor = uvec4(2, 0, 0, 1);
		return;
	}
	if(value == 3)
	{
		outColor = uvec4(3, 0, 0, 1);
		return;
	}
	if(value == 4)
	{
		outColor = uvec4(4, 0, 0, 1);
		return;
	}
	return;
}

// 0 = air
// 1 = sand
// 2 = water
// 3 = falling sand
// 4 = falling water
	
int getPixelValue(ivec2 offset, ivec2 pos)
{

	ivec2 size = textureSize(texture1, 0);
	ivec2 p = pos + offset;

	if (p.y < 0)
        return 1;

	if (p.x < size.x / 5 || p.x > size.x - 1)
	{
		if (offset.y > 0)
		{
			return 0;
		}
		return 1;
	}

	uint cell = texelFetch(texture1, p, 0).r;

	if(cell == uint(1))
	{
		return 1;
	}

	if(cell == uint(2))
	{
		return 2;
	}

	if(cell == uint(3))
	{
		return 3;
	}

	if(cell == uint(4))
	{
		return 4;
	}

	return 0;

}

uint hash(uvec2 p) 
{
	p = p * 1664525u + 1013904223u;
	return p.x ^ p.y;
}

void main()
{

	ivec2 pos = ivec2(gl_FragCoord.xy);

	ivec2 size = textureSize(texture1, 0);

	
	bool randLeftFirst = (hash(uvec2(pos)) & 1u) == 0u;

	int cell = getPixelValue(ivec2(0.0, 0.0), pos);
	
	int aboveCell = getPixelValue(ivec2(0.0, 1.0), pos);
	int aboveRightCell = getPixelValue(ivec2(1.0, 1.0), pos);
	int aboveLeftCell = getPixelValue(ivec2(-1.0, 1.0), pos);
	
	int belowCell = getPixelValue(ivec2(0.0, -1.0), pos);
	int belowRightCell = getPixelValue(ivec2(1.0, -1.0), pos);
	int belowLeftCell = getPixelValue(ivec2(-1.0, -1.0), pos);
	
	int rightCell = getPixelValue(ivec2(1.0, 0.0), pos);
	int leftCell = getPixelValue(ivec2(-1.0, 0.0), pos);

	if(distance(vec2(pos), mousePos) < 5 && leftButtonPressed && mousePos.x > size.x / 5 + 5)
	{
		newState(spawnColor);
		return;
	}
	
	if(cell == 0)
	{
		outColor = uvec4(0, 0, 0, 1);
	}
	if(cell == 1)
	{
		outColor = uvec4(1, 0, 0, 1);
	}
	if(cell == 2)
	{
		outColor = uvec4(2, 0, 0, 1);
	}
	if(cell == 3)
	{
		outColor = uvec4(3, 0, 0, 1);
	}
	if(cell == 4)
	{
		outColor = uvec4(4, 0, 0, 1);
	}
}

