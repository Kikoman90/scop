#version 410 core

layout(location = 0) in vec3	iPosition;
layout(location = 1) in vec3	iNormal;
layout(location = 2) in vec2	iUV;
layout(location = 3) in vec3	iColor;

uniform mat4	m;
uniform mat4	vp;

out vec3		vFragPos;
out vec3		vNormal;
out vec2		vUV;
out vec3		vColor;

void main()
{
	vFragPos = vec3(m * vec4(iPosition, 1.0f));
	gl_Position = vp * m * vec4(iPosition, 1.0f);
	vNormal = normalize(vec3(m * vec4(iNormal, 0.0f)));
	vUV = iUV;
	vColor = iColor;
}