/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_go.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 11:31:07 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/17 17:11:50 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	clean_obj(t_gameobject *go)
{
	if (go->vertices)
	{
		free(go->vertices);
		go->vertices = NULL;
	}
	if (go->indices)
	{
		free(go->indices);
		go->indices = NULL;
	}
	free(go->name);
	free(go);
	go = NULL;
}

void	clean_go_node(t_go_node *node)
{
	clean_obj(node->go);
	node->next = NULL;
	free(node);
	node = NULL;
}
