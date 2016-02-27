#version 330 core

in VsOut
{
    vec2 texCoord;
} vsOut;

out vec4 color;

uniform sampler2D tex;

float toSRGB(float value)
{
    if (value < 0.0031308)
        return 12.92 * value;
    return 1.055 * pow(value, 0.41666) - 0.055;
}

void main()
{
    vec4 texel = texture(tex, vsOut.texCoord);
    color = vec4(toSRGB(texel.r), toSRGB(texel.g), toSRGB(texel.b), 1.f);
}