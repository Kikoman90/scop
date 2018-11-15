/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 11:23:13 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/15 19:17:35 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <stdio.h>

// remove this file

void		display_quaternion(t_quaternion q, const char *msg)
{
	ft_putendl(msg);
	printf("{xyzw} : {%f, %f, %f, %f\n", q.v.x, q.v.y, q.v.z, q.w);
}

void		display_mat4x4(t_mat4x4 mat, const char *msg)
{
	unsigned int i;
	unsigned int j;

	j = 0;
	ft_putendl("ROWMAJOR");
	ft_putendl(msg);
	while (j < 4)
	{
		i = 0;
		printf("| ");
		while (i < 4)
		{
			printf("% 06.1f ", mat.m[4 * i + j]);
			i++;
		}
		printf("|\n");
		j++;
	}
	ft_putendl("-----------------");
}

void		display_vec4(const char *p, t_vec4 v)
{
	printf("%s : {%f, %f, %f, %f}\n", p, v.x, v.y, v.z, v.w);
}

void		display_vec3(const char *p, t_vec3 v)
{
	printf("%s : {%f, %f, %f}\n", p, v.x, v.y, v.z);
}

void		display_vec2(const char *p, t_vec2 v)
{
	printf("%s : {%f, %f}\n", p, v.x, v.y);
}

void		display_gameobject(t_gameobject *obj)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	printf("display object (name : %s)\n", obj->name);
	printf("material id : %d\n", obj->mtl_id);
	printf("vtx_count : %zu\n", obj->vertex_data.count);
	while (i < obj->vertex_data.count)
	{
		j = i + 3;
		while (i < j)
		{
			printf("vtx n%u\n", i);
			display_vec3("vtx_pos", obj->vtx_attrib[i].position);
			display_vec2("vtx_uv", obj->vtx_attrib[i].uv);
			display_vec3("vtx_nrm", obj->vtx_attrib[i].normal);
			display_vec4("vtx_clr", obj->vtx_attrib[i].color);
			i++;
		}
		printf("\n");
	}
}

void		display_go_list(t_go_node *list)
{
	t_go_node	*tmp;

	tmp = list;
	printf("display obj list\n");
	while (tmp)
	{
		printf("Gameobject ID: %u\n", tmp->id);
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
		printf("Material ID: %u\n", tmp->id);
		display_material(tmp->mtl);
		tmp = tmp->next;
	}
}
