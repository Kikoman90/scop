#version 410 core

in vec3             vUVW;

uniform samplerCube skybox;

out vec4            FragColor;

void main()
{    
    FragColor = texture(skybox, vUVW);
}