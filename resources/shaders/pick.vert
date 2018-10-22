#version 410 core

in vec3			iPosition;

uniform mat4	m;
uniform mat4	vp;

void main()
{
	vec4 worldPos = m * vec4(iPosition, 1.0);
	gl_Position = vp * worldPos;
}