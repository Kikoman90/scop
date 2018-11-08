/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attrib_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 13:04:29 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/08 19:32:27 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static t_vec3		parse_vtx_attrib(t_seed *v_seed, char *data, \
	unsigned int idx, char *v)
{
	unsigned int	i;
	unsigned int	seed;
	char			*word;

	i = 1;
	seed = v_seed->beginseed;
	while (i <= idx && i <= v_seed->count && seed < v_seed->endseed && \
		data[seed])
	{
		if ((word = ft_strword(data, &seed)) && ft_strcmp(word, v) == 0)
		{
			if (i == idx)
			{
				free(word);
				return (vec3_atof(data, &seed, 0));
			}
		}
		seed = skip_line(data, seed);
		if (v_seed->line++ && word)
			free(word);
		i++;
	}
	return ((t_vec3)VEC3_ZERO);
}

static unsigned int	get_indices(unsigned int (*out_indices)[3], \
	t_obj_parser_var *opv, t_parser *parser, unsigned int *seed)
{
	char	*word;

	if ((word = ft_strword(parser->data, seed)) != NULL)
	{
		(*out_indices)[0] = (unsigned int)ft_atoi(word);
		(*out_indices)[1] = (unsigned int)ft_atoi(ft_strchr(word, '/', 1));
		(*out_indices)[2] = (unsigned int)ft_atoi(ft_strnchr(word, '/', 2, 1));
		free(word);
		if ((*out_indices)[0] == 0 || (*out_indices)[0] > opv->v_seed[0].count \
			|| (*out_indices)[1] > opv->v_seed[1].count \
			|| (*out_indices)[2] > opv->v_seed[2].count)
			return (parser_error(IDX_ERROR, parser->fname, opv->f_seed.line));
		return (1);
	}
	return (0);
}

static unsigned int	parse_face(t_gameobject *go, t_obj_parser_var *opv, \
	t_parser *parser, unsigned int seed)
{
	unsigned int		i;
	unsigned int		idx[3];
	t_vtx_attrib		vtx;

	i = 0;
	while (i < opv->f_count)
	{
		if (!get_indices(&idx, opv, parser, &seed))
			return (0);
		vtx.position = parse_vtx_attrib(&opv->v_seed[0], parser->data, idx[0], "v");
		vtx.uv = vec2_v3(parse_vtx_attrib(&opv->v_seed[1], parser->data, idx[1], "vt"));
		vtx.normal = vec3_norm(parse_vtx_attrib(&opv->v_seed[2], parser->data, idx[2], "vn"));
		vtx.color = vec3_f(0.1f + (float)((opv->vtx_fill + i) / 3) * opv->color_delta);
		if (i < 3)
			go->vtx_attrib[opv->vtx_fill + i++] = vtx;
		else
		{
			go->vtx_attrib[opv->vtx_fill + i] = go->vtx_attrib[opv->vtx_fill];
			go->vtx_attrib[opv->vtx_fill + i].color = vtx.color;
			go->vtx_attrib[opv->vtx_fill + i + 1] = go->vtx_attrib[opv->vtx_fill + i - 1];
			go->vtx_attrib[opv->vtx_fill + i + 1].color = vtx.color;
			go->vtx_attrib[opv->vtx_fill + i + 2] = vtx;
			i += 3;
		}
	}
	opv->vtx_fill += opv->f_count;
	return (1);
}

unsigned int		parse_faces(t_gameobject *go, t_obj_parser_var *opv, \
	t_parser *parser, char *w)
{
	unsigned int	i;
	unsigned int	seed;
	t_seed			*f_seed;

	i = 0;
	f_seed = &opv->f_seed;
	seed = f_seed->beginseed;
	while (seed < f_seed->endseed && i < f_seed->count && parser->data[seed])
	{
		if ((w = ft_strword(parser->data, &seed)) && ft_strcmp(w, "f") == 0)
		{
			if ((opv->f_count = check_idx_count(parser->data, seed, 1)) < 3)
				return (parser_error(FACE_ERROR, parser->fname, f_seed->line));
			if (!parse_face(go, opv, parser, seed))
			{
				free(w);
				return (0);
			}
			i += opv->f_count;
		}
		seed = skip_line(parser->data, seed);
		if (f_seed->line++ && w)
			free(w);
	}
	return (1);
}
