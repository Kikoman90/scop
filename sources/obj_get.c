/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 12:24:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/30 14:12:51 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void            get_mouse_ray(t_env *env, t_ray *ray, int x, int y)
{

    t_vec2          viewport_size;
    t_vec2          coord;
    t_vec3          axes[3];

    coord.x = (2.0f * --x - env->win_env.win_w) / env->win_env.win_w;
    coord.y = (env->win_env.win_h - 2.0f * --y) / env->win_env.win_h;
    get_matrix_axes(axes, \
        mat4x4_transpose(quat_to_mat4x4(env->camera.transform.rotation)));
    viewport_size.y = tanf(env->camera.fov * M_PI / 360) * env->camera.znear;
    viewport_size.x = viewport_size.y * env->win_env.win_w / env->win_env.win_h;
    ray->origin = env->camera.transform.position;
	axes[2] = vec3_scale(axes[2], -1);
    axes[2] = vec3_add(ray->origin, vec3_scale(axes[2], env->camera.znear));
    axes[2] = vec3_add(axes[2], vec3_scale(axes[1], viewport_size.y * coord.y));
    axes[2] = vec3_add(axes[2], vec3_scale(axes[0], viewport_size.x * coord.x));
    ray->dir = vec3_norm(vec3_sub(axes[2], ray->origin));
}

void			get_model_matrices(t_go_node *go_list, t_mat4x4 *m)
{
	t_go_node		*tmp;
	unsigned int	i;

	tmp = go_list;
	i = 0;
	while (tmp)
	{
		m[i++] = go_trs(tmp->go->transform);
		tmp = tmp->next;
	}
}

unsigned int	get_mtl_id(t_mtl_node *list, char *mtl_name, \
	unsigned int mtl_offset)
{
	t_mtl_node	*tmp;

	tmp = list;
	while (tmp)
	{
		if (tmp->id > mtl_offset && ft_strcmp(mtl_name, tmp->mtl->name) == 0)
		{
			free(mtl_name);
			return (tmp->id);
		}
		tmp = tmp->next;
	}
	free(mtl_name);
	return (0);
}

t_material		*get_mtl(t_mtl_node *list, unsigned int id)
{
	t_mtl_node	*tmp;

	tmp = list;
	while (tmp)
	{
		if (tmp->id == id)
			return (tmp->mtl);
		tmp = tmp->next;
	}
	return (NULL);
}

t_gameobject	*get_gameobject(t_go_node *list, unsigned int id)
{
	t_go_node	*tmp;

	tmp = list;
	while (tmp)
	{
		if (tmp->id == id)
			return (tmp->go);
		tmp = tmp->next;
	}
	return (NULL);
}
