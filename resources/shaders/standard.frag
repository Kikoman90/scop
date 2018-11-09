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

//vec3 specular = light.intensity * (mat.expnt_spc / 100.0f) \
//* pow(max(dot(viewDir, reflectDir), 0.0f), 32) \

void	main()
{	
	float dist = (-length(vFragPos - light.position) + light.range) / light.range;
	vec3 lightDir = normalize(light.position - vFragPos);
	float diffuseCoefficient = dot(vNormal, lightDir);
	vec3 diffuse = light.intensity * max(diffuseCoefficient, 0.0f) * mat.clr_dif * light.color * dist;
	float specularCoefficient = 0.0;
	vec3 viewDir = normalize(viewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, vNormal);	
	if(diffuseCoefficient > 0.0)
    	specularCoefficient = pow(max(dot(viewDir, reflectDir), 0.0f), 256);
	vec3 specular = light.intensity * (mat.expnt_spc / 100.0f) * specularCoefficient * mat.clr_spc * light.color * dist;
	vec4 final = vec4(mat.clr_amb + diffuse + specular, mat.transp);
	FragColor = final;

	// mix (texture, final);
}