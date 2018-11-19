#version 410 core

layout (location = 0) in vec3 iPosition;

uniform mat4	m;
uniform mat4    v;
uniform mat4    p;

uniform mat4    rot[3]; // YOU NEED 4. gl_InstanceID will be from 0 to 3
uniform vec3    clr[4];
uniform bool    localspace;

// uniform vec3 pos;
// uniform mat4 rot;
// uniform vec3 scale;

// ca me parait pas mal
// uniform mat4 rot_matrix;
// uniform mat4 pos_scale_matrix;
// ca me parait bieng.

out vec3 vColor;

void main()
{
    if (localspace)
    {
        if (gl_InstanceID < 3)
            gl_Position = p * v * m * rot[gl_InstanceID] * vec4(iPosition, 1);
        else
            gl_Position = p * v * m * vec4(iPosition, 1);
    }
    else
    {
        if (gl_InstanceID < 3)
            gl_Position = p * v * rot[gl_InstanceID] * vec4(iPosition, 1);
        else
            gl_Position = p * v * vec4(iPosition, 1);
    }
    // iF abs(DOT(view_z, m * pos_dir)) > 0.95f => dont draw
    vColor = clr[gl_InstanceID];
}