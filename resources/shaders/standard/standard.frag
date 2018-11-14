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

uniform LightProps	light;
uniform MatProps	mat;
uniform vec3		view_pos;

out	vec4	FragColor;

void	main()
{	
	float dist = (-length(vFragPos - light.position) + light.range) / light.range;
	vec3 light_dir = normalize(light.position - vFragPos);
	float diffuse_coefficient = dot(vNormal, light_dir);
	vec3 diffuse = light.intensity * max(diffuse_coefficient, 0.0f) * mat.clr_dif * light.color * dist;
	float specular_coefficient = 0.0;
	vec3 view_dir = normalize(view_pos - vFragPos);
	vec3 reflect_dir = reflect(-light_dir, vNormal);	
	if(diffuse_coefficient > 0.0)
    	specular_coefficient = pow(max(dot(view_dir, reflect_dir), 0.0f), 256);
	vec3 specular = light.intensity * (mat.expnt_spc / 100.0f) * specular_coefficient * mat.clr_spc * light.color * dist;
	FragColor = vec4(mat.clr_amb + diffuse + specular, mat.transp);
}