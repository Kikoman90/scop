#version 410 core

layout(location = 0) in vec3    iPosition;

uniform mat4	v;
uniform mat4    p;

out vec3        vUVW;

void main()
{
    vUVW = vec3(-iPosition.x, iPosition.y, iPosition.z);
    mat4 view = v;
    view[3][0] = 0;
    view[3][1] = 0;
    view[3][2] = 0;
    vec4 pos = p * view * vec4(iPosition, 1.0);
    gl_Position = pos.xyww;
}