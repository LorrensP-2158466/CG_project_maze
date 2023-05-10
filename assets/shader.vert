#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
layout (std140) uniform PV_mats
{
    mat4 projection;
    mat4 view;
};

void main()
{
    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * norm; 
    TexCoords = tex;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}