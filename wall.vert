#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 coll;
layout (location = 2) in vec3 offset;
layout (location = 3) in vec2 texCoord;
layout (std140) uniform PV_mats
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;
out vec2 tCoord;
out vec3 colour;

void main()
{
    gl_Position = projection * view * model * vec4(pos + offset, 1.f); // + ofsset
    tCoord = vec2(texCoord);
    colour = coll;
}