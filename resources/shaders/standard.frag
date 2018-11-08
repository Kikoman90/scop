#version 410 core

in vec3		vFragPos;
in vec3		vNormal;

struct 		LightProps {
	vec3	position;
	vec3	color;
	float	intensity;
	float	range;
};

struct		MatProps {
	vec3	clr_amb;
	vec3	clr_dif;
	vec3	clr_spc;
	float	expnt_spc;
	float	transp;
};

uniform vec3		viewPos;
uniform LightProps	light;
uniform MatProps	mat;

out	vec4	FragColor;

void	main()
{
	vec3 ray = vFragPos - light.position;
	vec3 lightDir = normalize(light.position - vFragPos);
	float dist = -length(ray);
	dist = (dist + light.range) / light.range;
	vec3 diffuse = light.intensity * max(dot(vNormal, lightDir), 0.0f) \
		* mat.clr_dif * light.color * dist;
	vec3 viewDir = normalize(viewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, vNormal);
	vec3 specular = light.intensity * (mat.expnt_spc / 100.0f) \
	* pow(max(dot(viewDir, reflectDir), 0.0f), 32) \
	* mat.clr_spc * light.color * dist;
	vec3 final = mat.clr_amb + diffuse + specular;
	FragColor = vec4(final, mat.transp);
}