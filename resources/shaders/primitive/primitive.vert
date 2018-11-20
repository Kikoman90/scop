#version 410 core

layout (location = 0) in vec3 iPosition;

uniform mat4	m;
uniform mat4    v;
uniform mat4    p;

uniform mat4    rot[4];
uniform vec4    clr[4];
uniform bool    localspace;

//
// uniform mat4 rot_matrix;
// uniform mat4 pos_scale_matrix;
//

out vec4        vColor;

void main()
{
    if (localspace)
        gl_Position = p * v * m * rot[gl_InstanceID] * vec4(iPosition, 1);
    else
        gl_Position = p * v * rot[gl_InstanceID] * vec4(iPosition, 1); //FIX THIS
    
    // if (abs(dot(view_z, m * pos_dir)) > 0.95f)
    // => dont draw
    
    vColor = clr[gl_InstanceID];
}
