#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;
layout (location = 3) in mat4 instance_matrix;

out vec2 TexCoords;
layout (std140) uniform PV_mats
{
    mat4 projection;
    mat4 view;
};

void main()
{
    TexCoords = tex;
    gl_Position = projection * view * instance_matrix * vec4(pos, 1.0);
}