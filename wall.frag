#version 330 core

in vec2 tCoord;
in vec3 colour;
out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, tCoord);
}