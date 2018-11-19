/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 15:40:10 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/19 20:44:10 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void		remove_tr_node(t_tr_list *list, unsigned int id)
{
	t_tr_node   *tmp;
	t_tr_node   *tmp_prev;

	tmp = list->head;
	tmp_prev = tmp;
	while (tmp)
	{
		if (id == tmp->id)
		{
			if (tmp_prev == tmp)
				list->head = tmp_prev->next;
			else
				tmp_prev->next = tmp->next;
            free(tmp);
			tmp = NULL;
			list->count--;
			tmp = tmp_prev->next;
		}
		else
		{
			tmp_prev = tmp;
			tmp = tmp->next;
		}
	}
}

void			clear_tr_list(t_tr_list *list)
{
	t_tr_node	*tmp;
	t_tr_node	*tmp_next;

	if (list)
	{
		tmp = list->head;
		while (tmp)
		{
			tmp_next = tmp->next;
            tmp->next = NULL;
			free(tmp);
            tmp = NULL;
			list->count--;
			tmp = tmp_next;
		}
		list->head = NULL;
	}
}

t_tr_node       *create_tr_node(unsigned int id, t_transform *transform)
{
    t_tr_node   *node;

    if (!(node = (t_tr_node*)malloc(sizeof(t_tr_node))))
        return (log_error_null(MALLOC_ERROR));
    node->id = id;
    node->transform = transform;
    node->next = NULL;
    return (node);
}

unsigned int    add_to_selection(t_selection *selection, t_transform *transform, \
	unsigned int id, Uint8 lshift, const char *name)
{
    t_tr_node   *tmp;

    (void)name;
    if (!(tmp = selection->list.head) || !lshift)
    {
        printf("selection cleared\n");
        clear_tr_list(&selection->list);
        if (!(selection->list.head = create_tr_node(id, transform)))
            return (0);
    }
    else
    {
        while (tmp && tmp->next)
        {
            if (tmp->id == id)
            {
                remove_tr_node(&selection->list, id);
                printf("'%s' removed from selection [count = %zu]\n", name, selection->list.count);
                return (1);
            }
            printf("schweppes\n");
            tmp = tmp->next;
        }
        if (tmp->id == id)
        {
            remove_tr_node(&selection->list, id);
            printf("'%s' removed from selection [count = %zu]\n", name, selection->list.count);
            return (1);
        }
        else if (!(tmp->next = create_tr_node(id, transform)))
            return (0);
    }
    selection->list.count++;
    printf("'%s' added to selection [count = %zu]\n", name, selection->list.count);
    return (1);
}

void        selection_transform(t_selection *selection)
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
    tr.scale = vec3_f(0.1f);
    selection->transform = tr;
}

void		picking_check(t_env *env, int x, int y, Uint8 lshift)
{
	unsigned int	picked_id;
	unsigned char	data[3];
    t_gameobject    *go;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, env->buffers.pick_fbo);
	glReadPixels(x, env->win_env.win_h - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
	picked_id = data[0] + data[1] * 256 + data[2] * 256 * 256;
	if (picked_id == 1)
    {
        add_to_selection(&env->selection, &env->light.transform, 1, lshift, "light");
        selection_transform(&env->selection);
        //printf("calculate matrices and selection.transform\n");
    }
	else if (picked_id != 0 &&\
        (go = get_gameobject(env->gameobjects.head, picked_id)))
    {
        add_to_selection(&env->selection, &go->transform, picked_id, lshift, go->name);
        selection_transform(&env->selection);
        //printf("calculate matrices and selection.transform\n");
    }
    else if (!lshift)
    {
        clear_tr_list(&env->selection.list);
        printf("selection cleared\n");
    }
    display_vec3("selection_pos", env->selection.transform.position);
    display_quaternion(env->selection.transform.rotation, "selection_rot");
    display_vec3("selection_scale", env->selection.transform.scale);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}