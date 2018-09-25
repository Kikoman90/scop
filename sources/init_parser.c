/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 15:18:09 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/25 17:08:17 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

t_obj_parser_var	*init_obj_parser_var(t_obj_parser_var *opv, \
											char *name, unsigned int mtl_offset)
{
	if (!opv)
	{
		if (!(opv = (t_obj_parser_var*)malloc(sizeof(t_obj_parser_var))))
			return (log_error_null(MALLOC_ERROR));
	}
	opv->name = name;
	opv->mtl_id = 0;
	opv->mtl_offset = mtl_offset;
	opv->vtx_seed.beginseed = 0;
	opv->vtx_seed.endseed = 0;
	opv->vtx_seed.count = 0;
	opv->idx_seed.beginseed = 0;
	opv->idx_seed.endseed = 0;
	opv->idx_seed.count = 0;
	return (opv);
}

int					init_parser(t_parser *parser, const char *path, int *fd)
{
	struct stat	s;

	if ((*fd = open(path, O_RDWR)) == -1)
	{
		log_error_free(ft_strjoin("(open) ", strerror(errno)));
		return (-1);
	}
	if (fstat(*fd, &s) == -1)
	{
		close(*fd);
		log_error_free(ft_strjoin("(fstat) ", strerror(errno)));
		return (-1);
	}
	parser->fsize = (size_t)s.st_size;
	parser->fname = ft_strrchr(path, '/') + 1;
	parser->fpath = ft_strsub(path, 0, ft_strlen(path) \
								- ft_strlen(parser->fname));
	parser->fline = 1;
	parser->fseed = 0;
	parser->data = NULL;
	return (0);
}
