#version 410 core

in vec3	iPosition;
in vec3	iNormal;

uniform mat4	m;
uniform mat4	vp;

out vec4		vNormal;

void	main()
{
	vec4 worldPos = m * vec4(iPosition, 1.0);
	gl_Position = vp * worldPos;
	vNormal = m * vec4(iNormal, 1.0);
}