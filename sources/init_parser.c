/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 15:18:09 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/24 15:21:25 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

t_obj_parser_var	*init_opv(t_obj_parser_var *opv, char *name, \
								unsigned int mtl_offset)
{
	unsigned int i;

	if (!opv)
	{
		if (!(opv = (t_obj_parser_var*)malloc(sizeof(t_obj_parser_var))))
			return (log_error_null(MALLOC_ERROR));
	}
	opv->name = name;
	opv->mtl_id = 0;
	opv->mtl_offset = mtl_offset;
	i = 3;
	while (i--)
	{
		opv->v_seed[i].line = 0;
		opv->v_seed[i].beginseed = 0;
		opv->v_seed[i].endseed = 0;
		opv->v_seed[i].count = 0;
	}
	opv->f_seed.line = 0;
	opv->f_seed.beginseed = 0;
	opv->f_seed.endseed = 0;
	opv->f_seed.count = 0;
	opv->attrib_list = NULL;
	return (opv);
}

static int			init_parser(t_parser *parser, const char *path, int *fd)
{
	struct stat	file_stat;
	size_t		path_length;

	if ((*fd = open(path, O_RDWR)) == -1)
	{
		log_error_free(ft_strjoin("(open) ", strerror(errno)));
		return (-1);
	}
	if (fstat(*fd, &file_stat) == -1)
	{
		close(*fd);
		log_error_free(ft_strjoin("(fstat) ", strerror(errno)));
		return (-1);
	}
	parser->fsize = (size_t)file_stat.st_size;
	parser->fname = ft_strrchr(path, '/') + 1;
	path_length = ft_strlen(path) - ft_strlen(parser->fname);
	parser->fpath = ft_strsub(path, 0, path_length);
	parser->fline = 1;
	parser->fseed = 0;
	parser->data = NULL;
	return (0);
}

void			parse_file(t_env *env, const char *path, \
							void (*ft_parsing)(t_env*, t_parser*, char*))
{
	t_parser	*parser;
	int			fd;

	if (!(parser = (t_parser*)malloc(sizeof(t_parser))))
		log_error(MALLOC_ERROR);
	if (init_parser(parser, path, &fd) != -1)
	{
		if ((parser->data = (char *)mmap(NULL, parser->fsize, \
			PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			log_error_free(ft_strjoin("(mmap) ", strerror(errno)));
		else
		{
			parser->data[parser->fsize] = '\0';
			ft_parsing(env, parser, NULL);
			if (munmap(parser->data, parser->fsize) == -1)
				log_error_free(ft_strjoin("(munmap) ", strerror(errno)));
			parser->data = NULL;
		}
		close(fd);
		free(parser->fpath);
	}
	free(parser);
}
