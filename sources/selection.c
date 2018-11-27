/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 15:40:10 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/24 13:08:47 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void                set_selection_colors(t_selection *sel)
{
    if (sel->mode & SCOP_ROTATE)
        sel->colors[0] = vec4_v3w(vec3_norm((t_vec3)SCOP_GREY), 0.65f);
    else if (sel->mode & SCOP_SCALE)
        sel->colors[0] = vec4_v3w(vec3_norm((t_vec3)SCOP_GREY), 1.0f);
	sel->colors[1] = vec4_v3w(vec3_norm((t_vec3)SCOP_RED), 1.0f);
	sel->colors[2] = vec4_v3w(vec3_norm((t_vec3)SCOP_GREEN), 1.0f);
	sel->colors[3] = vec4_v3w(vec3_norm((t_vec3)SCOP_BLUE), 1.0f);
}

void                set_selection_mode(t_selection *sel, t_handlemode mode) // t_vec3 view_axis
{
    sel->mode = mode;
    if (sel->mode & SCOP_ROTATE)
    {
        sel->rot[0] = mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[2]);
        sel->rot[1] = mat4x4_trs(sel->offset[0], sel->quat[2], sel->scale[3]);
        sel->rot[2] = mat4x4_trs(sel->offset[0], sel->quat[1], sel->scale[3]);
        sel->rot[3] = mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[3]);
    }
    else if ((sel->mode & SCOP_TRANSLATE) || (sel->mode & SCOP_SCALE))
    {
        if (sel->mode & SCOP_SCALE)
            sel->rot[0] = \
                mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[2]);
        sel->rot[1] = mat4x4_trs(sel->offset[1], sel->quat[3], sel->scale[1]);
        sel->rot[2] = mat4x4_trs(sel->offset[2], sel->quat[0], sel->scale[1]);
        sel->rot[3] = mat4x4_trs(sel->offset[3], sel->quat[1], sel->scale[1]);
        sel->rot[4] = mat4x4_trs(sel->offset[0], sel->quat[3], sel->scale[0]);
        sel->rot[5] = mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[0]);
        sel->rot[6] = mat4x4_trs(sel->offset[0], sel->quat[1], sel->scale[0]);
    }
    //(void)view_axis;
    //set_selection_coplanars(sel, view_axis);
    set_selection_colors(sel);
}

void                set_selection_transform(t_selection *sel)
{
    t_transform tr;
    t_tr_node   *tmp;

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
        if (sel->localspace)
            tr.rotation = tmp->transform->rotation;
    }
    tr.position = vec3_scale(tr.position, 1.0f / sel->list.count);
    sel->transform = tr;
}

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

void		        picking_check(t_env *env, int x, int y, Uint8 lshift)
{
	unsigned int	picked_id;
	unsigned char	data[3];
    t_gameobject    *go;

    if (env->selection.active)
        return ;
    glBindFramebuffer(GL_READ_FRAMEBUFFER, env->buffers.pick_fbo);
	glReadPixels(x, env->win_env.win_h - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
	picked_id = data[0] + data[1] * 256 + data[2] * 256 * 256;
	if (picked_id == 1)
    {
        add_to_selection(&env->selection, &env->light.transform, 1, lshift);
        set_selection_transform(&env->selection);
    }
	else if (picked_id != 0 &&\
        (go = get_gameobject(env->gameobjects.head, picked_id)))
    {
        add_to_selection(&env->selection, &go->transform, picked_id, lshift);
        set_selection_transform(&env->selection);
    }
    else if (!lshift)
        clear_tr_list(&env->selection.list);
}
