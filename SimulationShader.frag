#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

uniform vec2 mousePos;
uniform bool leftButtonPressed;

void newState(float value)
{
	FragColor = vec4(value, value, 0.0, 1.0);
	return;
}
	
float getPixelValue(ivec2 offset, ivec2 pos)
{

	ivec2 size = textureSize(texture1, 0);
	ivec2 p = pos + offset;

	if (p.y < 0)
        return 1.0;

	if (p.x < size.x / 5 || p.x > size.x - 1)
	{
		if (offset.y > 0)
		{
			return 0.0;
		}
		return 1.0;
	}

	return texelFetch(texture1, p, 0).r;
}

void main()
{

	ivec2 pos = ivec2(gl_FragCoord.xy);

	ivec2 size = textureSize(texture1, 0);


	float cell = texelFetch(texture1, pos, 0).r;

	float aboveCell = getPixelValue(ivec2(0.0, 1.0), pos);
	float aboveRightCell = getPixelValue(ivec2(1.0, 1.0), pos);
	float aboveLeftCell = getPixelValue(ivec2(-1.0, 1.0), pos);

	float belowCell = getPixelValue(ivec2(0.0, -1.0), pos);
	float belowRightCell = getPixelValue(ivec2(1.0, -1.0), pos);
	float belowLeftCell = getPixelValue(ivec2(-1.0, -1.0), pos);

	if(distance(vec2(pos), mousePos) < 5 && leftButtonPressed && mousePos.x > size.x / 5 + 5)
	{
		newState(1);
		return;
	}

	if(cell > 0.0)
	{
		if(belowCell < 0.1 || ((belowRightCell < 0.1 || belowLeftCell < 0.1) && belowCell > 0.0))
		{
			newState(0.0);
			return;
		}
	}
	if(cell < 0.1)
	{
		if(aboveCell > 0.0)
		{
			newState(aboveCell);
			return;
		}
		if (aboveLeftCell > 0.0 && belowCell > 0.0)
		{
			newState(aboveLeftCell);
			return;
		}
		if(aboveRightCell > 0.0 && belowCell > 0.0)
		{
			newState(aboveRightCell);
			return;
		}
	}
	
	FragColor = vec4(cell, cell, 0.0, 1.0);
}

