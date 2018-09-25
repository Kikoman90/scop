/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 15:13:14 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/25 17:05:11 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../libs/libft/includes/libft.h"
# include "../libs/libmath/includes/libmath.h"

# define FILE_PREFIX_ERROR "invalid prefix in file "
# define MTL_UNSPECIFIED "cannot use material without loading .mtl file first"

typedef struct		s_seed
{
	unsigned int	beginseed;
	unsigned int	endseed;
	size_t			count;
}					t_seed;

typedef struct		s_obj_parser_var
{
	char			*name;
	unsigned int	mtl_id;
	unsigned int	mtl_offset;
	t_seed			vtx_seed;
	t_seed			idx_seed;
}					t_obj_parser_var;

typedef struct		s_parser
{
	unsigned int	fline;
	unsigned int	fseed;
	size_t			fsize;
	char			*fpath;
	const char		*fname;
	char			*data;
}					t_parser;

t_obj_parser_var	*init_obj_parser_var(t_obj_parser_var *opv, char *name, \
										unsigned int mtl_offset);
int					init_parser(t_parser *parser, const char *path, int *fd);

t_vec3				vec3_atof(char *data, unsigned int *seed);
unsigned int		skip_line(char *data, unsigned int seed);
unsigned int		check_idx_count(char *data, unsigned int seed, int idx);
char				*generate_name(char *name, const char *typename, \
									size_t count);

#endif
