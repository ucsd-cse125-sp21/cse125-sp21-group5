#version 330 core
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

	vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0);
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >= MAX_BONES) 
        {
            totalPosition = vec4(aPos, 1.0f);
            break;
        }
        vec4 localPosition = boneMatrices[boneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(boneMatrices[boneIds[i]]) * aNormal;
   }


    fragNormal = vec3(model * vec4(aNormal, 0));
    fragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = viewProj * model * vec4(aPos, 1.0);

    testColor = weights.xyz;
}