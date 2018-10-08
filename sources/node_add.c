/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_add.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 19:25:09 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/08 17:05:49 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

t_go_node			*add_go_node(t_env *env, t_go_node *node)
{
	t_go_node	*head;

	head = env->go_list;
	env->go_count++;
	if (head == NULL)
		return (node);
	while (env->go_list->next)
		env->go_list = env->go_list->next;
	env->go_list->next = node;
	return (head);
}

t_mtl_node			*add_mtl_node(t_env *env, t_mtl_node *node)
{
	t_mtl_node	*head;

	head = env->mtl_list;
	env->mtl_count++;
	if (head == NULL)
	{
		ft_putendl("here, you're supposed to go here");
		return (node);
	}
	while (env->mtl_list->next)
		env->mtl_list = env->mtl_list->next;
	ft_putendl("nope");
	env->mtl_list->next = node;
	return (head);
}
