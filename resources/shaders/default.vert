#version 410 core

in vec3			iPosition;
in vec3			iColor;
out vec4		vColor;
uniform vec3	uCenter;
uniform mat4	uMpv;

void main()
{
	vec3 position = iPosition - uCenter;
	gl_Position = uMpv * vec4(position, 1.0);
	vColor = vec4(iColor, 1);
}