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
	float dist = (-length(vFragPos - light.position) + light.range) / light.range;
	vec3 lightDir = normalize(light.position - vFragPos);
	float diffuseCoefficient = dot(vNormal, lightDir);
	vec3 diffuse = light.intensity * max(diffuseCoefficient, 0.0f) * vColor * light.color * dist;
	float specularCoefficient = 0.0;
	vec3 viewDir = normalize(viewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, vNormal);	
	if(diffuseCoefficient > 0.0)
    	specularCoefficient = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = light.intensity * specularCoefficient * vColor * light.color * dist;
	FragColor = vec4(diffuse + specular, 1.0f);
	
	//FragColor = vec4(vColor, 1.0f);
	//FragColor = mix(texture(TextureSampler, vUV).rgba, vColor, fade);
}