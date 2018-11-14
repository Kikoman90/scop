/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 19:55:18 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/14 19:55:45 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

t_vec3		generate_pick_clr(unsigned int id)
{
	float	r;
	float	g;
	float	b;

	r = (id & 0x000000FF) >> 0;
	g = (id & 0x0000FF00) >> 8;
	b = (id & 0x00FF0000) >> 16;
	return (vec3_xyz(r / 255.0f, g / 255.0f, b / 255.0f));
}

t_vec4      vtx_color(unsigned int fill, float color_delta)
{
	t_vec4	color;
	float	delta;

	color = vec4_f(0.1f);
	delta = (fill / 3.0f) * color_delta;
	color.w += delta;
	if ((fill + 1) % 3 == 0)
		color.x += delta;
	else if ((fill + 2) % 3 == 0)
		color.y += delta;
	else
		color.z += delta;
	return (color);
}