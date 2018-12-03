/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 15:40:10 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 18:35:02 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static unsigned int	add_to_selection(t_selection *selection, \
	t_transform *transform, unsigned int id, Uint8 lshift)
{
	t_tr_node	*tmp;

	if (!(tmp = selection->list.head) || !lshift)
	{
		clear_tr_list(&selection->list);
		if (!(selection->list.head = create_tr_node(id, transform)))
			return (0);
	}
	else
	{
		while (tmp && tmp->next)
		{
			if (tmp->id == id)
				return (remove_tr_node(&selection->list, id));
			tmp = tmp->next;
		}
		if (tmp && tmp->id == id)
			return (remove_tr_node(&selection->list, id));
		else if (!(tmp->next = create_tr_node(id, transform)))
			return (0);
	}
	selection->list.count++;
	return (1);
}

void				picking_check(t_env *env, int x, int y, Uint8 lshift)
{
	unsigned int	picked_id;
	unsigned char	data[3];
	t_gameobject	*go;

	if (env->selection.active)
		return ;
	glBindFramebuffer(GL_READ_FRAMEBUFFER, env->buffers.pick_fbo);
	glReadPixels(\
		x, env->win_env.win_h - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
	picked_id = data[0] + data[1] * 256 + data[2] * 256 * 256;
	if (picked_id == 1)
	{
		add_to_selection(&env->selection, &env->light.transform, 1, lshift);
		set_selection_transform(&env->selection, env->selection.localspace);
	}
	else if (picked_id != 0 &&\
		(go = get_gameobject(env->gameobjects.head, picked_id)))
	{
		add_to_selection(&env->selection, &go->transform, picked_id, lshift);
		set_selection_transform(&env->selection, env->selection.localspace);
	}
	else if (!lshift)
		clear_tr_list(&env->selection.list);
}

void				init_selection(t_selection *selection)
{
	selection->localspace = 1;
	selection->active = 0;
	selection->type = -1;
	selection->offset[0] = vec3_f(0);
	selection->offset[1] = vec3_xyz(8, 0, 0);
	selection->offset[2] = vec3_xyz(0, 8, 0);
	selection->offset[3] = vec3_xyz(0, 0, 8);
	selection->scale[0] = vec3_f(0.58f);
	selection->scale[1] = vec3_xyz(0, 1, 0);
	selection->scale[2] = vec3_xyz(0, 1, 0);
	selection->scale[3] = vec3_xyz(0, 1, 0);
	selection->scale[4] = vec3_f(0.9f);
	selection->scale[5] = vec3_f(0.905f);
	selection->quat[0] = quat();
	selection->quat[1] = quat_tv(-90, (t_vec3)VEC3_RIGHT);
	selection->quat[2] = quat_tv(90, (t_vec3)VEC3_UP);
	selection->quat[3] = quat_tv(90, (t_vec3)VEC3_FRONT);
	selection->coplanar[0] = 0;
	selection->coplanar[1] = 0;
	selection->coplanar[2] = 0;
	set_selection_mode(selection, SCOP_TRANSLATE, 1);
}
