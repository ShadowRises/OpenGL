#shader vertex
#version 330 core

in vec3 position;
in vec3 color;

out vec3 ex_color;

void main() {
        ex_color = color;
        gl_Position = vec4(position.xyz, 1.0);
}


#shader fragment
#version 330 core

in vec3 ex_color;
out vec4 FragColor;
 
void main() {
        FragColor = vec4(ex_color.xyz, 1.0f);
}
