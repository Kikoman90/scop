#version 410 core

in vec4				vNormal;
in vec2				vUV;
in vec4				vColor;

struct 				LightProps {
	vec3			position;
	vec3			color;
	float			intensity;
	float			range;
};

uniform vec3		viewPos;
uniform LightProps	light;
uniform float		fade;
uniform sampler2D	TextureSampler;

out vec4			FragColor;

void main()
{
	// lighting
	//

	//FragColor = mix(texture(TextureSampler, vUV).rgba, vColor, fade);
	FragColor = vec4(0.6f, 0.6f, 0.6f, fade);
}