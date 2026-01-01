#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec2 screenSize;

out vec4 FragColor;

void main()
{



	vec2 uv = gl_FragCoord.xy / screenSize;
	FragColor = vec4(texture(texture1, uv).x * 255, texture(texture1, uv).x * 255, 0.0, 1.0);
}