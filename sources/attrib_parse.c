/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attrib_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 13:04:29 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/14 13:38:28 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static t_vec3		get_vtx_attrib(t_seed *v_seed, char *data, \
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

static t_vtx_attrib	get_vtx(t_gameobject *go, t_obj_parser_var *opv, \
	char *data, unsigned int idx[4])
{
	static int		recalc = 0;
	t_vtx_attrib	vtx;

	vtx.position = get_vtx_attrib(&opv->v_seed[0], data, idx[0], "v");
	(vtx.position.x < go->bounds[0]) ? go->bounds[0] = vtx.position.x : 0;
	(vtx.position.x > go->bounds[1]) ? go->bounds[1] = vtx.position.x : 0;
	(vtx.position.y < go->bounds[2]) ? go->bounds[2] = vtx.position.y : 0;
	(vtx.position.y > go->bounds[3]) ? go->bounds[3] = vtx.position.y : 0;
	(vtx.position.z < go->bounds[4]) ? go->bounds[4] = vtx.position.z : 0;
	(vtx.position.z > go->bounds[5]) ? go->bounds[5] = vtx.position.z : 0;
	vtx.uv = vec2_v3(get_vtx_attrib(&opv->v_seed[1], data, idx[1], "vt"));
	vtx.normal = vec3_norm(get_vtx_attrib(&opv->v_seed[2], data, idx[2], "vn"));
	if (!recalc && vtx.normal.x == 0 && vtx.normal.y == 0 && vtx.normal.z == 0)
		recalc = 1;
	if (idx[3] > 1 && recalc-- == 1)
	{
		vtx.normal = vec3_norm(vec3_cross(vec3_sub(\
			go->vtx_attrib[opv->vtx_fill - idx[3]].position, \
			go->vtx_attrib[opv->vtx_fill - 1].position), vec3_sub(\
			go->vtx_attrib[opv->vtx_fill - idx[3]].position, vtx.position)));
		go->vtx_attrib[opv->vtx_fill - 2].normal = vtx.normal;
		go->vtx_attrib[opv->vtx_fill - 1].normal = vtx.normal;
	}
	vtx.color = vtx_color(opv->vtx_fill, opv->color_delta);
	return (vtx);
}


static unsigned int	get_indices(unsigned int (*out_indices)[4], \
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
	unsigned int		idx[4];
	t_vtx_attrib		vtx;

	idx[3] = 0;
	while (idx[3] < opv->f_count)
	{
		if (!get_indices(&idx, opv, parser, &seed))
			return (0);
		vtx = get_vtx(go, opv, parser->data, idx);
		if (idx[3] < 3)
			go->vtx_attrib[opv->vtx_fill++] = vtx;
		else
		{
			go->vtx_attrib[opv->vtx_fill] = \
				go->vtx_attrib[opv->vtx_fill - idx[3]];
			go->vtx_attrib[++opv->vtx_fill] = go->vtx_attrib[opv->vtx_fill - 2];
			opv->vtx_fill++;
			go->vtx_attrib[opv->vtx_fill++] = vtx;
			idx[3] += 2;
		}
		idx[3]++;
	}
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
