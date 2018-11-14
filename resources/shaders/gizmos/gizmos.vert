#version 410 core

layout(location = 0) in vec3    iPosition;
layout(location = 2) in vec2    iUV;

uniform mat4	m;
uniform mat4    v;
uniform mat4    p;

uniform bool    spherical;

out vec2        vUV;

void main()
{
    mat4 mv = v * m;
    mv[0][0] = 1;
    mv[0][1] = 0;
    mv[0][2] = 0;

    if (spherical)
    {
        mv[1][0] = 0;
        mv[1][1] = 1;
        mv[1][2] = 0;
    }

    mv[2][0] = 0;
    mv[2][1] = 0;
    mv[2][2] = 1;
	
    gl_Position = p * mv * vec4(iPosition, 1);
    vUV = iUV;
}