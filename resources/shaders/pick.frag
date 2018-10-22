#version 410 core

uniform vec3	pickClr;

out vec4		FragClr;

void main()
{
    FragClr = vec4(pickClr, 1.0f);
}