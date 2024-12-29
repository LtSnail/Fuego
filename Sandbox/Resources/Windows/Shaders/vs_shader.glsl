 #version 330 core
 layout(location = 0) in vec3 aPos; // Vertex position
 layout(location = 1) in vec3 color;    // Vertex color

 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;

 out vec3 fragColor; // Pass color to fragment shader

 void main()
 {
     gl_Position = projection * view * model * vec4(aPos, 1.0); 
     fragColor = color;                // Pass color to the fragment shader
 }
