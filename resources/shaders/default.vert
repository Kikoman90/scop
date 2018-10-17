#version 410 core

in vec3			iPosition;
//in vec3		iColor;

uniform mat4	mvp;

out vec4		vColor;

void main()
{
	gl_Position = mvp * vec4(iPosition, 1.0);
	vColor = vec4(0.7, 0.7, 0.7, 1);
	//vColor = vec4(iColor, 1);
}