#version 410 core

uniform vec3	pick_clr;

out vec4		FragColor;

void main()
{
    FragColor = vec4(pick_clr, 1.0f);
}