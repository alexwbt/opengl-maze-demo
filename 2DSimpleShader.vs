#version 460 core

layout (location = 0) in vec2 pos;

out vec3 Color;

uniform mat4 pvm;

void main()
{
    gl_Position = pvm * vec4(pos, 1, 1);

    Color = vec3(1, 1, 1);
}