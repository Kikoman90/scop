#version 410 core

in vec3				vFragPos;
in vec3				vNormal;
in vec2				vUV;
in vec4				vColor;

struct 				LightProps {
	vec3			position;
	vec3			color;
	float			intensity;
	float			range;
};

uniform LightProps	light;
uniform sampler2D	tex;
uniform vec3		view_pos;
uniform float		fade;
uniform bool		rgb;

out vec4			FragColor;

void main()
{
	vec3 f_color;
	if (rgb)
		f_color = vColor.rgb;
	else
		f_color = vec3(vColor.a);

	vec3 clr_mix = mix(texture(tex, vUV).rgb, f_color, fade);
	float alpha = mix(texture(tex, vUV).a, 1.0f, fade);

	float dist = (-length(vFragPos - light.position) + light.range) / light.range;
	vec3 light_dir = normalize(light.position - vFragPos);
	float diffuse_coefficient = dot(vNormal, light_dir);
	vec3 diffuse = light.intensity * max(diffuse_coefficient, 0.0f) * clr_mix * light.color * dist;
	
	float specular_coefficient = 0.0;
	vec3 view_dir = normalize(view_pos - vFragPos);
	vec3 reflect_dir = reflect(-light_dir, vNormal);	
	if(diffuse_coefficient > 0.0)
    	specular_coefficient = pow(max(dot(view_dir, reflect_dir), 0.0f), 256);
	vec3 specular = light.intensity * specular_coefficient * clr_mix * light.color * dist;
	FragColor = vec4(diffuse + specular, alpha);
}