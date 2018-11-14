/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 15:13:14 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/14 20:41:23 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../libs/libft/includes/libft.h"
# include "../libs/libmath/includes/libmath.h"

# define FILE_PREFIX_ERROR "invalid prefix in file "
# define IDX_ERROR "invalid index in file "
# define FACE_ERROR "invalid index count in file "

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
	unsigned int		vtx_fill;
	float				color_delta;
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

/*
** parser_utils.c		=> 2 functions
*/
t_vec3					vec3_atof(char *data, unsigned int *seed, int clamp01);
unsigned int			check_idx_count(char *data, unsigned int seed, int idx);

#endif
