/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 15:18:09 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/08 20:01:49 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static t_seed		init_seed_struct(void)
{
	t_seed	seed;

	seed.line = 0;
	seed.beginseed = 0;
	seed.endseed = 0;
	seed.count = 0;
	return (seed);
}

void				init_opv(t_obj_parser_var *opv, char *name, \
	unsigned int mtl_offset)
{
	opv->name = name;
	opv->mtl_id = 0;
	opv->mtl_offset = mtl_offset;
	opv->f_count = 0;
	opv->vtx_fill = 0;
	opv->v_seed[0] = init_seed_struct();
	opv->v_seed[1] = init_seed_struct();
	opv->v_seed[2] = init_seed_struct();
	opv->f_seed = init_seed_struct();
}

unsigned int		parser_error(const char *error, const char *fname, \
	unsigned int fline)
{
	char	*line_number;
	char	*error_msg;

	line_number = ft_strjoin_lf(ft_strjoin_rf(" (line ", ft_itoa(fline)), ")");
	error_msg = ft_strjoin_bf(ft_strjoin(error, "'"), \
		ft_strjoin_bf(ft_strjoin(fname, "'"), line_number));
	return (log_error_free(error_msg));
}

static unsigned int	init_parser(t_parser *parser, const char *path)
{
	parser->fpath = NULL;
	parser->data = NULL;
	if (!path || ft_strlen(path) == 0)
		return (log_error("invalid path"));
	if (!(parser->fname = ft_strrchr(path, '/')))
		parser->fname = path;
	else
	{
		parser->fname++;
		parser->fpath = ft_strsub(path, 0, \
			ft_strlen(path) - ft_strlen(parser->fname));
	}
	if (!(parser->data = ft_file_map(path, &parser->fsize)))
	{
		if (parser->fpath)
		{
			free(parser->fpath);
			parser->fpath = NULL;
		}
		return (0);
	}
	parser->data[parser->fsize] = '\0';
	parser->fline = 1;
	parser->fseed = 0;
	return (1);
}

void				parse_file(t_go_list *gameobjects, t_mtl_list *materials, \
	const char *path, void (*parse)(t_go_list*, t_mtl_list*, t_parser*, char*))
{
	t_parser	parser;

	if (init_parser(&parser, path))
	{
		parse(gameobjects, materials, &parser, NULL);
		ft_file_unmap(parser.data, parser.fsize, parser.fpath);
	}
}
