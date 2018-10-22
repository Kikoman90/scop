#version 410 core

in vec4		vNormal;

uniform 	LightProps {
	vec3	position;
	vec3	color;
	float	intensity;
	float	range;
};

uniform 	MatProps {
	vec3	clr_amb;
	vec3	clr_dif;
	vec3	clr_spc;
	float	expnt_spc;
	float	transp;
};

out	vec4	FragColor;

void	main()
{
	// lighting	
}