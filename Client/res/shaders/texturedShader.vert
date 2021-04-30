#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;

out vec3 fragNormal;
out vec2 texCoord;
out vec3 fragPos;

void main()
{
    //fragNormal = vec3(model * vec4(aNormal, 0));
    fragNormal = aNormal;
    texCoord = aTexCoord;
    fragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = viewProj * model * vec4(aPos, 1.0);
}