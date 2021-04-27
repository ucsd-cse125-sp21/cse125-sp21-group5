#version 330 core

in vec3 fragNormal;
in vec2 texCoord;

uniform sampler2D TEX_diffuse;

out vec4 fragColor;

void main()
{
	vec3 sun = normalize(vec3(1, 2, 0));

	// super basic lighting
	fragColor = texture(TEX_diffuse, texCoord) * (max(0, dot(fragNormal, sun)) + 0.2);
}

	