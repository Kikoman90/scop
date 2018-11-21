/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 15:40:10 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/21 20:46:04 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static unsigned int add_to_selection(t_selection *selection, \
    t_transform *transform, unsigned int id, Uint8 lshift)
{
    t_tr_node   *tmp;

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

void                selection_def_colors(t_vec4 (*colors)[4])
{
    (*colors)[0] = vec4_v3w(vec3_norm((t_vec3)SCOP_GREY), 0.7f);
	(*colors)[1] = vec4_v3w(vec3_norm((t_vec3)SCOP_RED), 1.0f);
	(*colors)[2] = vec4_v3w(vec3_norm((t_vec3)SCOP_GREEN), 1.0f);
	(*colors)[3] = vec4_v3w(vec3_norm((t_vec3)SCOP_BLUE), 1.0f);
}

void                selection_transform(t_selection *selection)
{
    t_transform tr;
    t_tr_node   *tmp;

    tr = init_transform();
    tmp = selection->list.head;
    while (tmp && tmp->next)
    {
        tr.position = vec3_add(tr.position, tmp->transform->position);
        tmp = tmp->next;
    }
    if (tmp)
    {
        tr.position = vec3_add(tr.position, tmp->transform->position);
        tr.rotation = tmp->transform->rotation;
    }
    tr.position = vec3_scale(tr.position, 1.0f / selection->list.count);
    tr.scale = (t_vec3)VEC3_ONE;
    selection->transform = tr;
}

void		        picking_check(t_env *env, int x, int y, Uint8 lshift)
{
	unsigned int	picked_id;
	unsigned char	data[3];
    t_gameobject    *go;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, env->buffers.pick_fbo);
	glReadPixels(x, env->win_env.win_h - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
	picked_id = data[0] + data[1] * 256 + data[2] * 256 * 256;
	if (picked_id == 1)
        add_to_selection(&env->selection, &env->light.transform, 1, lshift);
	else if (picked_id != 0 &&\
        (go = get_gameobject(env->gameobjects.head, picked_id)))
        add_to_selection(&env->selection, &go->transform, picked_id, lshift);
    else if (!lshift)
        clear_tr_list(&env->selection.list);
}
