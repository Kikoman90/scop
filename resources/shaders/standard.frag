#version 410 core

//in vec4				vColor;
//in vec2				vUV;
out vec4			FragColor;
//uniform sampler2D	TextureSampler;
uniform vec3        uPickColor;

void main()
{
	//FragColor = mix(texture(TextureSampler, vUV).rgba, vColor, uAlpha);
	//FragColor = vec4(vColor.rgb, uAlpha);
    FragColor = vec4(uPickColor, 1.0f);
}