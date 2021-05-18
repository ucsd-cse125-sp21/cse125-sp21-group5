#version 330 core

in vec3 fragNormal;
in vec2 texCoord;
in vec3 fragPos;

in float visibility;

uniform sampler2D TEX_diffuse;
uniform vec3 aViewPos;
uniform vec3 aViewDir;
uniform vec3 aFogColor;

// this mirrors the class in Renderer.h
struct PointLight {
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};

#define NUM_POINT_LIGHTS 32
uniform PointLight pointlights[NUM_POINT_LIGHTS];

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

uniform DirectionalLight sunLight;

out vec4 fragColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 sun = normalize(vec3(1, 2, 0));

	// super basic lighting
    vec3 color = vec3(0);

    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        color += CalcPointLight(pointlights[i], fragNormal, fragPos, aViewDir);
    }

    color += CalcDirectionalLight(sunLight, fragNormal, aViewDir);

    //fragColor = vec4(color, 1);
    fragColor = vec4(mix(aFogColor, color, visibility), 1);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			            light.quadratic * (distance * distance));    
    // combine results
    vec3 diffuse = light.color * diff * vec3(texture(TEX_diffuse, texCoord));
    diffuse *= attenuation;
    return diffuse;
} 

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = light.direction;
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // combine results
    vec3 diffuse = light.color * diff * vec3(texture(TEX_diffuse, texCoord));
    return diffuse;
} 
	