/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attrib_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 13:04:29 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 19:01:57 by fsidler          ###   ########.fr       */
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
			i++;
		}
		seed = skip_line(data, seed);
		if (v_seed->line++ && word)
			free(word);
	}
	return ((t_vec3)VEC3_ZERO);
}

static t_vtx_attrib	vtx_color(t_vtx_attrib *vtx, unsigned int fill, \
	float color_delta)
{
	float	delta;

	vtx->color = vec4_f(0.1f);
	delta = (fill / 3.0f) * color_delta;
	vtx->color.w += delta;
	if ((fill + 1) % 3 == 0)
		vtx->color.x += delta;
	else if ((fill + 2) % 3 == 0)
		vtx->color.y += delta;
	else
		vtx->color.z += delta;
	return (*vtx);
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
	if (idx[3] > 1 && recalc == 1)
	{
		vtx.normal = vec3_norm(vec3_cross(vec3_sub(\
			go->vtx_attrib[opv->vtx_fill - idx[3]].position, \
			go->vtx_attrib[opv->vtx_fill - 1].position), vec3_sub(\
			go->vtx_attrib[opv->vtx_fill - idx[3]].position, vtx.position)));
		go->vtx_attrib[opv->vtx_fill - 2].normal = vtx.normal;
		go->vtx_attrib[opv->vtx_fill - 1].normal = vtx.normal;
		recalc = 0;
	}
	return (vtx_color(&vtx, opv->vtx_fill, opv->color_delta));
}

static unsigned int	get_indices(unsigned int out_indices[4], \
	t_obj_parser_var *opv, t_parser *parser, unsigned int *seed)
{
	char	*word;

	if ((word = ft_strword(parser->data, seed)) != NULL)
	{
		out_indices[0] = (unsigned int)ft_atoi(word);
		out_indices[1] = (unsigned int)ft_atoi(ft_strchr(word, '/', 1));
		out_indices[2] = (unsigned int)ft_atoi(ft_strnchr(word, '/', 2, 1));
		free(word);
		if (out_indices[0] == 0 || out_indices[0] > opv->v_seed[0].count \
			|| out_indices[1] > opv->v_seed[1].count \
			|| out_indices[2] > opv->v_seed[2].count)
			return (parser_error(IDX_ERROR, parser->fname, opv->f_seed.line));
		return (1);
	}
	return (0);
}

unsigned int		parse_face(t_gameobject *go, t_obj_parser_var *opv, \
	t_parser *parser, unsigned int seed)
{
	unsigned int		idx[4];
	t_vtx_attrib		vtx;

	idx[3] = 0;
	while (idx[3] < opv->f_count)
	{
		if (!get_indices(idx, opv, parser, &seed))
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
