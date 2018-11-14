/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 12:24:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/14 21:42:16 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

int				get_shader_idx(t_shader (*tab)[6], const char *name)
{
	unsigned int	i;

	i = 0;
	while (i < 6)
	{
		if (ft_strcmp(name, (*tab)[i].name) == 0)
			return (i);
		i++;
	}
	printf("-1\n");
	return (-1);
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
