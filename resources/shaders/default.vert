#version 410 core

in vec3	iPosition;
in vec3	iNormal;
in vec2	iUV;
in vec3	iColor;

uniform mat4	m;
uniform mat4	vp;

out vec4		vNormal;
out vec2		vUV;
out vec4		vColor;

void main()
{
	vec4 worldPos = m * vec4(iPosition, 1.0);
	gl_Position = vp * worldPos;
	vColor = vec4(iColor, 1);
	vNormal = m * vec4(iNormal, 1.0);
	vUV = iUV;
}