/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:43:49 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/31 13:41:39 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void			rotate_gameobjects(t_go_node *list, double delta)
{
	t_quaternion	z_rot;
	t_go_node		*tmp;

	z_rot = quat_tv(22.5f * delta, (t_vec3)VEC3_UP);
	tmp = list;
	while (tmp)
	{
		tmp->go->transform.rotation = \
			quat_mult(tmp->go->transform.rotation, z_rot);
		tmp = tmp->next;
	}
}

unsigned int    mat_update(t_mat4x4 **mat, size_t count)
{
    if (*mat)
	{
        free (*mat);
		*mat = NULL;
	}
    if (count == 0)
    	return (0);
    count++;
    if (!(*mat = (t_mat4x4*)malloc(sizeof(t_mat4x4) * count)))
        return (1);
    return (0);
}

t_mat4x4		compute_view(t_camera cam)
{
	t_mat4x4	view;

	view = mat4x4_transpose(quat_to_mat4x4(cam.transform.rotation));
	view.m[12] = -vec3_dot(vec3_v4(view.v[0]), cam.transform.position);
	view.m[13] = -vec3_dot(vec3_v4(view.v[1]), cam.transform.position);
	view.m[14] = -vec3_dot(vec3_v4(view.v[2]), cam.transform.position);
	return (view);
}

// not here
t_mat4x4	    compute_proj(float fov, float aspect, float zn, float zf)
{
	t_mat4x4	proj_mat;
	float f;

	fov *= M_PI / 180.0f;
	f = 1 / tanf(fov / 2);
	proj_mat = mat4x4();
	proj_mat.m[0] = f / aspect;
	proj_mat.m[5] = f;
	proj_mat.m[10] = -(zf + zn) / (zf - zn);
	proj_mat.m[11] = -1;
	proj_mat.m[14] = (-2 * zn * zf) / (zf - zn);
	proj_mat.m[15] = 0;
	return (proj_mat);
}