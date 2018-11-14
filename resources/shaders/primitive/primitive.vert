#version 410 core

// line loop for circles
// line_strip for lines 
// cubes, cones, spheres => drawelements();
// draw elements instanced (color as per instance parameter)
// draw instanced ?? send colors.

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColor;

uniform mat4	m;
uniform mat4    v;
uniform mat4    p;

uniform bool    localspace;

out vec3 vColor;

void main()
{
    if (localspace)
        gl_Position = p * v * m * vec4(iPosition, 1);
    else
        gl_Position = p * v * vec4(iPosition, 1);
    vColor = iColor;
}