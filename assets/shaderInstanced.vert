#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;
layout (location = 3) in mat4 instance_matrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 viewPos;
uniform mat4 model;
layout (std140) uniform PV_mats
{
    mat4 projection;
    mat4 view;
    vec3 cam_pos;
};

void main()
{
    viewPos = cam_pos;
    FragPos = vec3(instance_matrix * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(instance_matrix))) * norm; 
    TexCoords = tex;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
