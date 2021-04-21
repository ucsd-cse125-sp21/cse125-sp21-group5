#version 330 core

in vec3 fragNormal;
in vec2 texCoord;

uniform sampler2D TEX_diffuse;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
	vec3 sun = normalize(vec3(1, 2, 0));

	vec4 color = texture(TEX_diffuse, texCoord) * (max(0, dot(fragNormal, sun)) * 2 + 0.25);
	fragColor = color;

	//vec3 color = vec3(1.0, 0.2, 0.2);

	//vec3 color = (0.5 * fragNormal) + 0.5;
	//fragColor = vec4(color, 1);

	//fragColor = texture(TEX_diffuse, texCoord);
}

	