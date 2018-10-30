#version 410 core

layout(location = 0) in vec3	iPosition;
layout(location = 1) in vec3	iNormal;
layout(location = 2) in vec2	iUV;
layout(location = 3) in vec3	iColor;

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