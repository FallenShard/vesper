#version 330 core

in VsOut
{
    vec2 texCoord;
} vsOut;

out vec4 color;

uniform sampler2D tex;

void main()
{
    color = texture(tex, vsOut.texCoord);
}