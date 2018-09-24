/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_add.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:25:09 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/24 19:41:21 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

t_go_node			*add_go_node(t_go_node *dest, t_go_node *node)
{
	t_go_node	*head;

	head = dest;
	if (head == NULL)
		return (node);
	while (dest->next)
		dest = dest->next;
	dest->next = node;
	return (head);
}

t_mtl_node			*add_mtl_node(t_mtl_node *dest, t_mtl_node *node)
{
	t_mtl_node	*head;

	head = dest;
	if (head == NULL)
		return (node);
	while (dest->next)
		dest = dest->next;
	dest->next = node;
	return (head);
}
