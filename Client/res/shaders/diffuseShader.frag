#version 330 core

in vec3 fragNormal;
in vec2 texCoord;

uniform vec3 aColor;

out vec4 fragColor;

void main()
{
	vec3 sun = normalize(vec3(1, 2, 0));

	// super basic lighting
	fragColor = vec4(aColor, 1) * (max(0, dot(fragNormal, sun)) + 0.2);
}

	