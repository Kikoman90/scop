#version 410 core

layout(location = 0) in vec3	iPosition;
layout(location = 1) in vec3	iNormal;

uniform mat4	m;
uniform mat4	vp;

out vec3		vNormal;
out vec3		vFragPos;

void	main()
{
	vFragPos = vec3(m * vec4(iPosition, 1.0));
	gl_Position = vp * m * vec4(iPosition, 1.0);
	vNormal = normalize(vec3(m * vec4(iNormal, 0.0)));
}