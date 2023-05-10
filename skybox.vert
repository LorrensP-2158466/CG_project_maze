#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
layout (std140) uniform PV_mats
{
    mat4 projection;
    mat4 view;
};

void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
} 