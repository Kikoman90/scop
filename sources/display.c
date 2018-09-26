/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 11:23:13 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/17 11:23:39 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <stdio.h>

// remove this file

void		display_vec3(const char *p, t_vec3 v)
{
	printf("%s : {%f, %f, %f}\n", p, v.x, v.y, v.z);
}

void		display_gameobject(t_gameobject *obj)
{
	unsigned int i;

	i = 0;
	printf("display object (name : %s)\n", obj->name);
	printf("material id : %d\n", obj->mtl_id);
	/*while (i < obj->vtx_count)
	{
		printf("vertex %d = (%f, %f, %f)\n", i, obj->vertices[i].x, obj->vertices[i].y, obj->vertices[i].z);
		i++;
	}
	i = 0;
	while (i < obj->idx_count)
	{
		printf("triangle %d = (%d, %d, %d)\n", i / 3, obj->indices[i], obj->indices[i + 1], obj->indices[i + 2]);
		i += 3;
	}*/
}

void		display_go_list(t_go_node *list)
{
	t_go_node	*tmp;

	tmp = list;
	printf("display obj list\n");
	while (tmp)
	{
		display_gameobject(tmp->go);
		tmp = tmp->next;
	}
}

void		display_material(t_material *mtl)
{
	unsigned int i;

	i = 0;
	printf("display material (name : %s)\n", mtl->name);
	display_vec3("clr_amb", mtl->clr_amb);
	display_vec3("clr_dif", mtl->clr_dif);
	display_vec3("clr_spc", mtl->clr_spc);
	printf("expnt_spc : %f\n", mtl->expnt_spc);
	printf("transparency : %f\n", mtl->transparency);
}

void		display_mtl_list(t_mtl_node *list)
{
	t_mtl_node	*tmp;

	tmp = list;
	printf("display mtl list\n");
	while (tmp)
	{
		display_material(tmp->mtl);
		tmp = tmp->next;
	}
}
