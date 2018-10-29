/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 15:13:14 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/29 11:48:19 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../libs/libft/includes/libft.h"
# include "../libs/libmath/includes/libmath.h"

# define FILE_PREFIX_ERROR "invalid prefix in file "
# define INVALID_IDX_ERROR "invalid index in file "
# define FACE_ERROR "invalid index count in file "

typedef struct			s_idx_attrib
{
	unsigned int		attrib[3];
	unsigned int		idx_ret;
	struct s_idx_attrib	*next;
}						t_idx_attrib;

typedef struct			s_seed
{
	unsigned int		line;
	unsigned int		beginseed;
	unsigned int		endseed;
	size_t				count;
}						t_seed;

typedef struct			s_obj_parser_var
{
	char				*name;
	unsigned int		mtl_id;
	unsigned int		mtl_offset;
	unsigned int		f_count;
	unsigned int		attrib_fill;
	t_idx_attrib		*attrib_list;
	t_seed				f_seed;
	t_seed				v_seed[3];
}						t_obj_parser_var;

typedef struct			s_parser
{
	unsigned int		fline;
	unsigned int		fseed;
	size_t				fsize;
	const char			*fname;
	char				*fpath;
	char				*data;
}						t_parser;

t_vec3					vec3_atof(char *data, unsigned int *seed, int clamp01);
unsigned int			skip_line(char *data, unsigned int seed);
unsigned int			check_idx_count(char *data, unsigned int seed, int idx);

t_idx_attrib			*free_attrib(t_idx_attrib *list);

#endif
