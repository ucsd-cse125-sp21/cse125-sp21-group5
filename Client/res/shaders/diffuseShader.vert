#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;

out vec3 fragNormal;

void main()
{
    fragNormal = vec3(model * vec4(aNormal, 0));
    gl_Position = viewProj * model * vec4(aPos, 1.0);
}