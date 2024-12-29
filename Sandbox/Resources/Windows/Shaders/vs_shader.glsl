 #version 330 core
 layout(location = 0) in vec3 position; // Vertex position
 layout(location = 1) in vec3 color;    // Vertex color


 out vec3 fragColor; // Pass color to fragment shader

 void main()
 {
     gl_Position = vec4(position, 1.0); 
     fragColor = color;                // Pass color to the fragment shader
 }
