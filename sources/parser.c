/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 20:10:35 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/12 21:27:57 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static int	read_file(const char *file, int *fd)
{
	if ((*fd = open(file, O_RDWR)) == -1)
	{
		log_error_free(ft_strjoin("(open) ", strerror(errno)));
		return (-1);
	}
	return (*fd);
}

static void	skip_line(char *data, int *seed)
{
	while (data[*seed] && data[*seed] != '\n')
		*seed += 1;
	if (data[*seed] == '\n')
		*seed += 1;
}

static void	prefix_error(const char *fname, unsigned int fline)
{
	char	*line_number;
	char	*error_msg;

	line_number = ft_strjoin2(ft_strjoin3(" (line ", ft_itoa(fline)), ")");
	error_msg = ft_strjoin2(ft_strjoin(FILE_PREFIX_ERROR, "'"), \
			ft_strjoin2(ft_strjoin(fname, "'"), line_number));
	free(line_number);
	log_error_free(error_msg);
}

t_gameobject	*create_go(const char *name)
{
	t_gameobject	*go;

	if (!(go = (t_gameobject*)malloc(sizeof(t_gameobject))))
		return (log_error_null(MALLOC_ERROR));
	go->transform = NULL; //go->transform = identityMatrix();
	go->vertices = NULL;
	go->indices = NULL;
	go->mtl = NULL;
	go->name = name;
	return (go);
}

t_go_node		*create_go_node(const char *name)
{
	t_go_node	*node;

	if (!(node = (t_go_node*)malloc(sizeof(t_go_node))))
		return (log_error_null(MALLOC_ERROR));
	node->go = create_go(name);
	node->next = NULL;
	return (node);
}

t_gameobject	*add_gameobject(t_go_node *list, const char *go_name)
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

static void	parse_wavefrontobj(t_env *env, int seed)
{
	char			*w;
	t_gameobject	*bound_go;

	bound_go = NULL;
	while (seed < env->parser->fsize && env->parser->data[seed])
	{
		w = ft_strword(env->parser->data, &seed);
		if (ft_strcmp(w, "#") == 0)
			skip_line(env->parser->data, &seed);
		else if (ft_strcmp(w, "o") == 0)
			bound_go = add_gameobject(env->obj_list, w);
		else if (ft_strcmp(w, "v") == 0)
			printf("vertex\n");
		else if (ft_strcmp(w, "f") == 0)
		{
			//add indices to indexbuffer
		}
		else if (ft_strcmp(w, "mtllib") == 0)
		{
			//load Mtlfile
		}
		else if (ft_strcmp(w, "usemtl") == 0)
		{
			//fetch mtl
		}
		else
		{
			prefix_error(env->parser->fname, env->parser->fline);
			skip_line(env->parser->data, &seed);
		}
		env->parser->fline++;
	}
}

void		parse_file(t_env *env, const char *path)
{
	int	fd;

	if (read_file(path, &fd) != -1 && \
		(env->parser->fsize = file_size(fd)) != -1)
	{
		if ((env->parser->data = (char *)mmap(NULL, env->parser->fsize, \
			PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			log_error_free(ft_strjoin("(mmap) ", strerror(errno)));
		else
		{
			env->parser->data[env->parser->fsize - 1] = '\0';
			parse_wavefrontobj(env, 0);
			if (munmap(env->parser->data, env->parser->fsize) == -1)
				log_error_free(ft_strjoin("(munmap) ", strerror(errno)));
			env->parser->data = NULL;
		}
		close(fd);
	}
}
