#version 410 core

uniform 	MatProps {
	vec3	clr_amb;
	vec3	clr_dif;
	vec3	clr_spc;
	float	expnt_spc;
	float	transparency;
};

uniform 	LightProps {
	vec3	position;
	vec3	color;
	float	intensity;
	float	range;
};

//uniform sampler2D	TextureSampler;

in	vec4	iNormal;
out	vec4	FragColor;

void	main()
{
	//FragColor = mix(texture(TextureSampler, vUV).rgba, vColor, uAlpha);
}