#version 410 core

layout(location = 0) in vec3			iPosition;

uniform mat4	m;
uniform mat4	v;
uniform mat4	p;

uniform bool    billboard;

void main()
{
	mat4 mv = v * m; // could send mv directly as a uniform
	if (billboard)
	{
		mv[0][0] = 1;
    	mv[0][1] = 0;
    	mv[0][2] = 0;
		mv[1][0] = 0;
        mv[1][1] = 1;
		mv[1][2] = 0;
    	mv[2][0] = 0;
    	mv[2][1] = 0;
    	mv[2][2] = 1;
	}
	gl_Position = p * mv * vec4(iPosition, 1.0);
}
