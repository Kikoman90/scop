#version 410 core

in vec4				vNormal;
in vec2				vUV;
in vec4				vColor;

uniform 			LightProps {
	vec3			position;
	vec3			color;
	float			intensity;
	float			range;
};

uniform float		fade;
uniform sampler2D	TextureSampler;

out vec4			FragColor;

void main()
{
	// lighting
	FragColor = mix(texture(TextureSampler, vUV).rgba, vColor, fade);
}