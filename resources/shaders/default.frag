#version 410 core

in vec4				vColor;
in vec2				vUV;
out vec4			FragColor;
uniform sampler2D	TextureSampler;
uniform float		uAlpha;
void main()
{
	FragColor = mix(texture(TextureSampler, vUV).rgba, vColor, uAlpha);
}