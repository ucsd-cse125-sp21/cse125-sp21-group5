#version 430 core

layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aNormal;
layout (location = 2) in vec2  aTexCoord;
layout (location = 3) in ivec4 aBoneIds; 
layout (location = 4) in vec4  aWeights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 boneMatrices[MAX_BONES];

// Uniform variables
uniform mat4 aViewProj;
uniform vec3 aViewPos;
uniform mat4 aModel;

uniform float aFogDensity;
uniform float aFogGradient;

out vec3 fragNormal;
out vec2 texCoord;
out vec3 fragPos;

out vec3 testColor;
out float visibility;

float calculateFog(float d, float density, float gradient);

void main()
{
    vec3 totalPosition = vec3(0);
    vec3 totalNormal = vec3(0);

    testColor = vec3(0);

    if (aBoneIds[0] < 0) {
		// if the first index is -1, we know that this vertex must have no bone weights
		totalPosition = aPos;
		totalNormal = aNormal;
    }
    else {
		for (int i = 0; i < 4; i++) {
			if (aBoneIds[i] < 0) {
				testColor = vec3(1, 1, 0);
				continue;
			}

			totalPosition += aWeights[i] * vec3(boneMatrices[aBoneIds[i]] * vec4(aPos, 1));
			totalNormal += aWeights[i] * vec3(boneMatrices[aBoneIds[i]] * vec4(aNormal, 0));
		}
    }

	texCoord = aTexCoord;
    fragNormal = normalize(vec3(aModel * vec4(totalNormal, 0)));
    fragPos = vec3(aModel * vec4(totalPosition, 1));
    gl_Position = aViewProj * aModel * vec4(totalPosition, 1);

	// calculate fog
	float fogDensity = aFogDensity;
	float fogGradient = aFogGradient;
	//visibility = calculateFog(length(fragPos - aViewPos), fogDensity, fogGradient);
	visibility = calculateFog(length(fragPos - aViewPos), fogDensity, fogGradient);
}

float calculateFog(float d, float density, float gradient) 
{
	return clamp(exp(-pow(d * density, gradient)), 0, 1);
}
