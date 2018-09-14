/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 20:10:35 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/14 15:06:49 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

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

	if (init_parser(env->parser, path, &fd) != -1)
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
		}
		close(fd);
	}
}
