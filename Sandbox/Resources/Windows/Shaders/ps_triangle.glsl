#version 330 core

uniform sampler2D sampler;

in vec2 TextCoord;
vec4 FragColor;

void main()
{
    FragColor = texture2D(sampler, TextCoord);
}
