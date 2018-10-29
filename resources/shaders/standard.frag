#version 410 core

in vec4		vNormal;

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

out	vec4	FragColor;

void	main()
{
	//uniforms that are not used return -1 with glGetUniformLocation();
	//vec4 sum = vec4(light.color.x, light.color.y, light.color.z, 1.0f);
	//float range_intensity = light.intensity + light.range;
	//vec4 pos = vec4(light.position, 0.2f);
	// lighting
	FragColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
}