#version 410 core

layout (location = 0) in vec3 iPosition;

uniform mat4	m;
uniform mat4    v;
uniform mat4    p;

uniform mat4    rot[4];
uniform vec4    clr[4];

out vec4        vColor;

void main()
{
    gl_Position = p * v * m * rot[gl_InstanceID] * vec4(iPosition, 1);
    
    // vec3 dir = 
    // if (abs(dot(vec3(v[2]), m * pos_dir)) > 0.95f)
    // => dont draw
    
    vColor = clr[gl_InstanceID];
}
