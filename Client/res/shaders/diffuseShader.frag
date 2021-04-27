#version 330 core

in vec3 fragNormal;
in vec2 texCoord;

uniform vec3 aColor;

out vec4 fragColor;

void main()
{
	// super basic lighting
	fragColor = vec4(aColor, 1);
}

	