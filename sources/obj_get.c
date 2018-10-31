/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 12:24:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/31 13:55:38 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void			get_model_matrices(t_go_node *go_list, t_mat4x4 *m)
{
	t_go_node	*tmp;

	tmp = go_list;
	while (tmp)
	{
		m[tmp->id] = go_trs(tmp->go->transform);
		tmp = tmp->next;
	}
}

unsigned int    get_mtl_id(t_env *env, char *mtl_name, unsigned int mtl_offset)
{
	t_mtl_node  *mtl_list;

	mtl_list = env->mtl_list;
	while (mtl_list)
	{
		if (mtl_list->id > mtl_offset && \
			ft_strcmp(mtl_name, mtl_list->mtl->name) == 0)
		{
			free(mtl_name);
			return (mtl_list->id);
		}
		mtl_list = mtl_list->next;
	}
	free(mtl_name);
	return (0);
}

t_material		*get_mtl(t_mtl_node *list, unsigned int id)
{
	t_mtl_node	*tmp;

	tmp = list;
	while (tmp != NULL)
	{
		if (id == tmp->id)
			return (tmp->mtl);
		tmp = tmp->next;
	}
	return (NULL);
}

t_gameobject	*get_gameobject(t_go_node *list, unsigned int id)
{
	t_go_node	*tmp;

	tmp = list;
	while (tmp != NULL)
	{
		if (id == tmp->id)
			return (tmp->go);
		tmp = tmp->next;
	}
	return (NULL);
}