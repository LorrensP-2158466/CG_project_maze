#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec3 offset;

layout (std140) uniform PV_mats
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

out vec3 colour; // output a color to the fragment shader


vec3 random (vec3 st) {
    return vec3(fract(sin(dot(st.xyz, vec3(12.9898,78.233, 40.151)))*43758.5453123));
}

void main()
{
    gl_Position = projection * view * model * vec4(pos + offset, 1.f); // + ofsset
    colour = col;

}