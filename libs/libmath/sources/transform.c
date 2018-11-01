/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 15:51:15 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/01 21:38:57 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_mat4x4	go_trs(t_transform tr)
{
	return (mat4x4_trs(tr.position, tr.rotation, tr.scale));
}

t_transform	init_transform(void)
{
	t_transform	tr;

	tr.position = (t_vec3)VEC3_ZERO;
	tr.rotation = quat();
	tr.scale = (t_vec3)VEC3_ONE;
	return (tr);
}

t_transform	init_transform_trs(t_vec3 t, t_quaternion r, t_vec3 s)
{
	t_transform	tr;

	tr.position = t;
	tr.rotation = r;
	tr.scale = s;
	return (tr);
}
