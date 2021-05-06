#version 430 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 tex;
layout (location = 3) in ivec4 boneIds; 
layout (location = 4) in vec4 weights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 boneMatrices[MAX_BONES];

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;

out vec3 fragNormal;
out vec3 fragPos;

out vec3 testColor;

void main()
{
    vec3 totalPosition = vec3(0);
    vec3 totalNormal = vec3(0);

    testColor = vec3(0);

    for (int i = 0; i < 4; i++) {
        if (boneIds[i] < 0) {
			testColor = vec3(1, 1, 0);
            continue;
        }

        totalPosition += weights[i] * vec3(boneMatrices[boneIds[i]] * vec4(aPos, 1));
        totalNormal += weights[i] * vec3(boneMatrices[boneIds[i]] * vec4(aNormal, 0));
    }

    //vec4 v1 = boneMatrices[1] * vec4(aPos, 1);

    /*
    for (int i = 0; i < 2; i++) {
        vec3 localPosition = vec3(boneMatrices[boneIds[i]] * vec4(aPos, 1));
        totalPosition += localPosition * weights[i];
    }
    */

    /*
    fragNormal = vec3(model * vec4(aNormal, 0));
    fragPos = vec3(model * vec4(totalPosition, 1));
    gl_Position = viewProj * model * vec4(totalPosition, 1);
    */

    fragNormal = vec3(model * vec4(totalNormal, 0));
    fragPos = vec3(model * vec4(totalPosition, 1));
    gl_Position = viewProj * model * vec4(totalPosition, 1);
}