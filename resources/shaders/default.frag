#version 410 core

in vec3				vFragPos;
in vec3				vNormal;
in vec2				vUV;
in vec3				vColor;

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
	vec3 ray = vFragPos - light.position;
	vec3 lightDir = normalize(light.position - vFragPos);
	float dist = -length(ray);
	dist = (dist + light.range) / light.range;
	vec3 diffuse = light.intensity * max(dot(vNormal, lightDir), 0.0f) \
		* vColor * light.color * dist;
	vec3 viewDir = normalize(viewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, vNormal);
	vec3 specular = light.intensity \
	* pow(max(dot(viewDir, reflectDir), 0.0f), 32) \
	* vColor * light.color * dist;
	FragColor = vec4(diffuse + specular, 1.0f);
	//FragColor = vec4(vColor, 1.0f);
	//FragColor = mix(texture(TextureSampler, vUV).rgba, vColor, fade);
}