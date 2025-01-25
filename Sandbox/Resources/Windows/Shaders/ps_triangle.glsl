#version 330 core

in vec2 text_coords;

uniform sampler2D gSampler;

out vec4 FragColor;

void main()
{
    FragColor = texture2D(gSampler, text_coords);
}
