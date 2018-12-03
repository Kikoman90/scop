/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection_set.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 12:35:16 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 18:36:31 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	reset_selection_aspect(t_selection *sel)
{
	sel->offset[1] = vec3_xyz(8, 0, 0);
	sel->offset[2] = vec3_xyz(0, 8, 0);
	sel->offset[3] = vec3_xyz(0, 0, 8);
	sel->scale[1] = vec3_xyz(0, 1, 0);
	sel->scale[2] = vec3_xyz(0, 1, 0);
	sel->scale[3] = vec3_xyz(0, 1, 0);
	set_selection_mode(sel, sel->mode, 1);
}

void	set_selection_transform(t_selection *sel, int localspace)
{
	t_transform	tr;
	t_tr_node	*tmp;

	sel->localspace = localspace;
	tr.position = (t_vec3)VEC3_ZERO;
	tr.rotation = quat();
	tmp = sel->list.head;
	while (tmp && tmp->next)
	{
		tr.position = vec3_add(tr.position, tmp->transform->position);
		tmp = tmp->next;
	}
	if (tmp)
	{
		tr.position = vec3_add(tr.position, tmp->transform->position);
		if (sel->localspace || sel->mode & SCOP_SCALE)
			tr.rotation = tmp->transform->rotation;
	}
	tr.position = vec3_scale(tr.position, 1.0f / sel->list.count);
	tr.scale = sel->transform.scale;
	sel->transform = tr;
}

void	set_selection_colors(t_selection *sel)
{
	if (sel->mode & SCOP_ROTATE)
		sel->colors[0] = vec4_v3w(vec3_norm((t_vec3)SCOP_GREY), 0.65f);
	else if (sel->mode & SCOP_SCALE)
		sel->colors[0] = vec4_v3w(vec3_norm((t_vec3)SCOP_GREY), 1.0f);
	sel->colors[1] = vec4_v3w(vec3_norm((t_vec3)SCOP_RED), 1.0f);
	sel->colors[2] = vec4_v3w(vec3_norm((t_vec3)SCOP_GREEN), 1.0f);
	sel->colors[3] = vec4_v3w(vec3_norm((t_vec3)SCOP_BLUE), 1.0f);
}

void	set_selection_mode(t_selection *sel, t_handlemode mode, int set_colors)
{
	sel->mode = mode;
	if (sel->mode & SCOP_ROTATE)
	{
		sel->rot[0] = mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[4]);
		sel->rot[1] = mat4x4_trs(sel->offset[0], sel->quat[2], sel->scale[5]);
		sel->rot[2] = mat4x4_trs(sel->offset[0], sel->quat[1], sel->scale[5]);
		sel->rot[3] = mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[5]);
	}
	else if ((sel->mode & SCOP_TRANSLATE) || (sel->mode & SCOP_SCALE))
	{
		if (sel->mode & SCOP_SCALE)
			sel->rot[0] = \
				mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[4]);
		sel->rot[1] = mat4x4_trs(sel->offset[1], sel->quat[3], sel->scale[0]);
		sel->rot[2] = mat4x4_trs(sel->offset[2], sel->quat[0], sel->scale[0]);
		sel->rot[3] = mat4x4_trs(sel->offset[3], sel->quat[1], sel->scale[0]);
		sel->rot[4] = mat4x4_trs(sel->offset[0], sel->quat[3], sel->scale[1]);
		sel->rot[5] = mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[2]);
		sel->rot[6] = mat4x4_trs(sel->offset[0], sel->quat[1], sel->scale[3]);
	}
	(set_colors) ? set_selection_colors(sel) : 0;
	set_selection_transform(sel, sel->localspace);
}
