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

void				init_seeds(t_seed *vtx_seed, t_seed *idx_seed)
{
	vtx_seed->beginseed = -1;
	vtx_seed->endseed = -1;
	vtx_seed->count = 0;
	idx_seed->beginseed = -1;
	idx_seed->endseed = -1;
	idx_seed->count = 0;
}

char				*generate_go_name(unsigned int count)
{
	count++;
	return (ft_strjoin_rf(GAMEOBJECT_NAME, ft_itoa((int)count)));
}

static t_gameobject	*create_gameobject(char *name)
{
	t_gameobject	*go;

	if (!(go = (t_gameobject*)malloc(sizeof(t_gameobject))))
	{
		free(name);
		return (log_error_null(MALLOC_ERROR));
	}
	// go->transform = identityMatrix(); // create vtx frag shaders, create matrix functions and clean libmath
	// write rendering functions, vao vbo and ibo (idx buffer object). drawElements();
	go->vertices = NULL;
	go->indices = NULL;
	go->vtx_count = 0;
	go->idx_count = 0;
	go->mtl = NULL;
	go->name = name;
	return (go);
}

t_go_node		*create_go_node(char *name)
{
	t_go_node	*node;

	if (!(node = (t_go_node*)malloc(sizeof(t_go_node))))
	{
		free(name);
		return (log_error_null(MALLOC_ERROR));
	}
	if (!(node->go = create_gameobject(name)))
		return (NULL);
	node->next = NULL;
	return (node);
}

t_go_node			*add_go_node(t_env *env, t_go_node *node)
{
	t_go_node	*head;

	head = env->obj_list;
	env->obj_count++;
	if (head == NULL)
		return (node);
	while (env->obj_list->next)
		env->obj_list = env->obj_list->next;
	env->obj_list->next = node;
	return (head);
}
