#version 410 core

in vec2         vUV;

uniform sampler2D   tex;

out vec4		FragColor;

void main()
{
    FragColor = texture(tex, vUV).rgba;
}