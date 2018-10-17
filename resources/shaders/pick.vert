#version 410 core

in vec3			iPosition;

uniform mat4	mvp;

void main()
{
	gl_Position = mvp * vec4(iPosition, 1.0);
}