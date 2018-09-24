/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 16:48:51 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/18 16:48:53 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	clean_go(t_gameobject *go)
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

static void	clean_mtl(t_material *mtl)
{
	free(mtl->name);
	free(mtl);
	mtl = NULL;
}

void		clean_go_node(t_go_node *node)
{
	clean_go(node->go);
	node->next = NULL;
	free(node);
	node = NULL;
}

void		clean_mtl_node(t_mtl_node *node)
{
	clean_mtl(node->mtl);
	node->next = NULL;
	free(node);
	node = NULL;
}
