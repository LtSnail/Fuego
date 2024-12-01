#version 330 core
layout (location = 0) in vec3 aPos;   // Position attribute
layout (location = 1) in vec3 aColor; // Color attribute

out vec3 ourColor; // Output color to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor; // Pass the color to the fragment shader
}
