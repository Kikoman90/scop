/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 16:48:51 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/16 21:34:41 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void		clean_mtl_node(t_mtl_node *node, int free_mtl)
{
	if (free_mtl && node->mtl)
	{
		if (node->mtl->name)
			free(node->mtl->name);
		free(node->mtl);
		node->mtl = NULL;
	}
	node->next = NULL;
	free(node);
}

void		remove_mtl_node(t_mtl_list *list, unsigned int id, int free_mtl)
{
	t_mtl_node		*tmp;
	t_mtl_node		*tmp_prev;

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
			clean_mtl_node(tmp, free_mtl);
			id = 0;
			list->count--;
			tmp = tmp_prev->next;
		}
		else
		{
			tmp->id -= (id == 0) ? 1 : 0;
			tmp_prev = tmp;
			tmp = tmp->next;
		}
	}
}

void		clean_go_node(t_go_node *node, int free_go)
{
	if (free_go && node->go)
	{
		if (node->go->vtx_attrib)
			free(node->go->vtx_attrib);
		if (node->go->name)
			free(node->go->name);
		if (node->go->vbo)
			glDeleteBuffers(1, &node->go->vbo);
		if (node->go->vao)
			glDeleteVertexArrays(1, &node->go->vao);
		free(node->go);
		node->go = NULL;
	}
	node->next = NULL;
	free(node);
}

void		remove_go_node(t_go_list *list, unsigned int id, int free_go)
{
	t_go_node		*tmp;
	t_go_node		*tmp_prev;

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
			clean_go_node(tmp, free_go);
			id = 0;
			list->count--;
			tmp = tmp_prev->next;
		}
		else
		{
			tmp->id -= (id == 0) ? 1 : 0;
			tmp_prev = tmp;
			tmp = tmp->next;
		}
	}
	// need to update model matrices
}
