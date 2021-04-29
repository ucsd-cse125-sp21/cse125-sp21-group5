#version 330 core

in vec3 fragNormal;
in vec3 fragPos;

uniform vec3 aColor;
uniform vec3 aViewPos;
uniform vec3 aViewDir;

// LIGHTING

// this mirrors the class in Renderer.h
struct PointLight {
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};

#define NUM_POINT_LIGHTS 4
uniform PointLight pointlights[NUM_POINT_LIGHTS];

out vec4 fragColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 sun = normalize(vec3(1, 2, 0));
	// super basic lighting

    //fragColor = vec4(pointlights[0].color, 1);
    fragColor = vec4(normalize(aViewDir), 1);

	//fragColor = vec4(aColor, 1) * (max(0, dot(fragNormal, sun)) + 0.2);

    vec3 color = vec3(0);

    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        color += CalcPointLight(pointlights[i], fragNormal, fragPos, aViewDir);
    }

    fragColor = vec4(color, 1);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // I HACKED THIS TOGETHER DON'T USE THIS LONG TERM
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 diffuse = light.color * diff * aColor;
    diffuse *= attenuation;
    return diffuse;
} 
	