#version 330 core
layout(location = 0) out uvec4 outColor;

uniform usampler2D texture1;
uniform sampler2D texture2;

uniform vec2 mousePos;
uniform bool leftButtonPressed;
uniform int spawnColor;

#define Air 0
#define Sand 1
#define FallingSand 2
#define Water 3
#define FallingWater 4
#define Gas 5
#define RisingGas 6

void newState(int value)
{
	if(value == Air)
	{
		outColor = uvec4(0, 0, 0, 1);
		return;
	}
	if(value == Sand)
	{
		outColor = uvec4(1, 0, 0, 1);
		return;
	}
	if(value == FallingSand)
	{
		outColor = uvec4(2, 0, 0, 1);
		return;
	}
	if(value == Water)
	{
		outColor = uvec4(3, 0, 0, 1);
		return;
	}
	if(value == FallingWater)
	{
		outColor = uvec4(4, 0, 0, 1);
		return;
	}
	if(value == Gas)
	{
		outColor = uvec4(5, 0, 0, 1);
		return;
	}
	if(value == RisingGas)
	{
		outColor = uvec4(6, 0, 0, 1);
		return;
	}
	return;
}

// 0 = air
// 1 = sand
// 2 = falling sand
// 3 = water
// 4 = falling water
// 5 = gas
// 6 = rising gas
	
int getPixelValue(ivec2 offset, ivec2 pos)
{

	ivec2 size = textureSize(texture1, 0);
	ivec2 p = pos + offset;

	if (p.y < 0)
        return Sand;
	if (p.y >= size.y)
		return Gas;
	if (p.x < size.x / 5 || p.x > size.x - 1)
	{
		if (offset.y > 0)
		{
			return Air;
		}
		return Sand;
	}

	uint cell = texelFetch(texture1, p, 0).r;

	if(cell == uint(Sand))
	{
		return Sand;
	}

	if(cell == uint(FallingSand))
	{
		return FallingSand;
	}

	if(cell == uint(Water))
	{
		return Water;
	}

	if(cell == uint(FallingWater))
	{
		return FallingWater;
	}

	if(cell == uint(Gas))
	{
		return Gas;
	}

	if(cell == uint(RisingGas))
	{
		return RisingGas;
	}

	return Air;

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
		if(cell == Air)
		{
			newState(spawnColor);
			return;
		}
	}
	
	if(cell == Sand)
	{
		if(belowCell == Air)
		{
			newState(FallingSand);
			return;
		}
	}

	if(cell == Water)
	{
		if(belowCell == Air)
		{
			newState(FallingWater);
			return;
		}
		if(aboveCell == FallingSand)
		{
			newState(FallingSand);
			return;
		}
	}

	if(cell == Gas)
	{
		if(belowCell == Air)
		{
			newState(RisingGas);
			return;
		}
	}

	if(cell == FallingSand)
	{
		if(belowCell == Sand)
		{
			newState(Sand);
			return;
		}
		if(belowCell == Water)
		{
			newState(Water);
			return;
		}
		if(aboveCell == Air || aboveCell == Gas || aboveCell == RisingGas)
		{
			newState(Air);
			return;
		}
	}

	if(cell == FallingWater)
	{
		if(belowCell == Sand || belowCell == Water)
		{
			newState(Water);
			return;
		}
		if(aboveCell == Air || aboveCell == Gas || aboveCell == RisingGas)
		{
			newState(Air);
			return;
		}
	}

	if(cell == RisingGas)
	{
		if(aboveCell == Gas)
		{
			newState(Gas);
			return;
		}
		if(belowCell != Gas && belowCell != RisingGas)
		{
			newState(Air);
			return;
		}
	}

	if(cell == Air)
	{
		if(aboveCell == FallingSand)
		{
			newState(FallingSand);
			return;
		}
		if(aboveCell == FallingWater)
		{
			newState(FallingWater);
			return;
		}
		if(belowCell == RisingGas)
		{
			newState(RisingGas);
			return;
		}
	}

	if(cell == Air)
	{
		outColor = uvec4(0, 0, 0, 1);
	}
	if(cell == Sand)
	{
		outColor = uvec4(1, 0, 0, 1);
	}
	if(cell == FallingSand)
	{
		outColor = uvec4(2, 0, 0, 1);
	}
	if(cell == Water)
	{
		outColor = uvec4(3, 0, 0, 1);
	}
	if(cell == FallingWater)
	{
		outColor = uvec4(4, 0, 0, 1);
	}
	if(cell == Gas)
	{
		outColor = uvec4(5, 0, 0, 1);
	}
	if(cell == RisingGas)
	{
		outColor = uvec4(6, 0, 0, 1);
	}
}

