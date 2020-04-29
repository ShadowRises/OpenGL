#shader vertex
#version 330 core

in vec3 position;
in vec3 color;
in vec2 texture_coord;

out vec3 ex_color;
out vec2 ex_tex_coord;

void main() {
        ex_color = color;
        ex_tex_coord = texture_coord;
        gl_Position = vec4(position,
                           1.f);
}


#shader fragment
#version 330 core

in vec3 ex_color;
in vec2 ex_tex_coord;
out vec4 FragColor;

uniform sampler2D texture_data1;
uniform sampler2D texture_data2;

void main() {
       // FragColor = vec4(ex_color, 1.0f);
       // FragColor = texture(texture_data, ex_tex_coord);// * vec4(ex_color, 1.0f);
        FragColor = mix(
                texture(texture_data1, ex_tex_coord),
                texture(texture_data2, ex_tex_coord),
                0.2);
}
