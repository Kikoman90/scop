#version 410 core

in vec3			iPosition;
//in vec3			iColor;
out vec4		vColor;
//uniform vec3	uCenter;
uniform mat4	mpv;

void main()
{
	//vec3 position = iPosition - uCenter; //positions should already be local (not world)
	gl_Position = mpv * vec4(position, 1.0);
	vColor = vec4(0.7, 0.7, 0.7, 1);
	//vColor = vec4(iColor, 1);
}