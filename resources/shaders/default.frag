#version 410 core

in vec4				vColor;
//in vec2			vUV;

uniform float		uAlpha;
//uniform sampler2D	TextureSampler;

out vec4			FragColor;

void main()
{
	//FragColor = mix(texture(TextureSampler, vUV).rgba, vColor, uAlpha);
	FragColor = vec4(vColor.rgb, uAlpha);
}