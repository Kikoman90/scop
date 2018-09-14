/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 15:51:02 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/14 15:04:25 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static t_gameobject	*create_go(const char *name)
{
	t_gameobject	*go;

	if (!(go = (t_gameobject*)malloc(sizeof(t_gameobject))))
		return (log_error_null(MALLOC_ERROR));
	// go->transform = identityMatrix();
	go->vertices = NULL;
	go->indices = NULL;
	go->mtl = NULL;
	go->name = name;
	return (go);
}

static t_go_node	*create_go_node(const char *name)
{
	t_go_node	*node;

	if (!(node = (t_go_node*)malloc(sizeof(t_go_node))))
		return (log_error_null(MALLOC_ERROR));
	node->go = create_go(name);
	node->next = NULL;
	return (node);
}

t_gameobject		*add_gameobject(t_go_node *list, const char *go_name)
{
	t_go_node	*tmp;

	if (!list)
	{
		list = create_go_node(go_name);
		tmp = list;
	}
	else
	{
		tmp = list;
		while (tmp)
			tmp = tmp->next;
		tmp = create_go_node(go_name);
	}
	return (tmp->go);
}
