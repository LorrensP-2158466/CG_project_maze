#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec3 colour; // output a color to the fragment shader

uniform mat4 model;
layout (std140) uniform PV_mats
{
    mat4 projection;
    mat4 view;
};


void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.f);
    colour = col;

}