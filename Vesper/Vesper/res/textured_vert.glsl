#version 330 core

layout(location = 0) in vec2 position;
layout(location = 2) in vec2 texCoord;

out VsOut
{
    vec2 texCoord;
} vsOut;

void main()
{
    gl_Position = vec4(position, 0.f, 1.f);
    vsOut.texCoord = texCoord;
}