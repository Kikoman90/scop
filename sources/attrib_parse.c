/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attrib_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 13:04:29 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/01 21:47:58 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static t_vec3		feed_attrib(t_seed *v_seed, char *data, unsigned int idx, \
	char *v)
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

void				parse_vtx_attrib(t_gameobject *go, t_obj_parser_var *opv, \
	char *data)
{
	unsigned int	i;
	t_idx_attrib	*tmp;

	i = 0;
	tmp = opv->attrib_list;
	while (tmp && i < go->vtx_count)
	{
		if (i == tmp->idx_ret)
		{
			go->vtx_attrib[i].vertex = feed_attrib(&opv->v_seed[0], \
				data, tmp->attrib[0], "v");
			go->vtx_attrib[i].uv = vec2_v3(feed_attrib(&opv->v_seed[1], \
				data, tmp->attrib[1], "vt"));
			go->vtx_attrib[i].normal = vec3_norm(feed_attrib(&opv->v_seed[2], \
				data, tmp->attrib[2], "vn"));
			go->vtx_attrib[i].color = vec3_xyz(\
				sinf(go->vtx_attrib[i].vertex.x), \
				sinf(go->vtx_attrib[i].vertex.y), \
				sinf(go->vtx_attrib[i].vertex.z));
			i++;
		}
		tmp = tmp->next;
	}
}

static unsigned int	*get_ret_indices(t_obj_parser_var *opv, char *data, \
	unsigned int seed)
{
	unsigned int	i;
	unsigned int	*ret;
	char			*word;
	t_idx_attrib	f_tmp;

	if (!(ret = (unsigned int*)malloc(sizeof(unsigned int) * opv->f_count)))
		return (log_error_null(MALLOC_ERROR));
	i = 0;
	while (i < opv->f_count)
	{
		word = ft_strword(data, &seed);
		f_tmp.attrib[0] = (unsigned int)ft_atoi(word);
		f_tmp.attrib[1] = (unsigned int)ft_atoi(ft_strchr(word, '/', 1));
		f_tmp.attrib[2] = (unsigned int)ft_atoi(ft_strnchr(word, '/', 2, 1));
		if (word)
			free(word);
		if (!check_attrib_list(opv, &f_tmp, &ret[i]))
		{
			free(ret);
			return (NULL);
		}
		i++;
	}
	return (ret);
}

static unsigned int	feed_indices(t_gameobject *go, unsigned int *indices, \
	unsigned int i, unsigned int count)
{
	unsigned int	j;
	unsigned int	ri;
	unsigned int	ii;

	if (!indices)
		return (0);
	ii = 0;
	ri = i + 3;
	while (i < ri)
		go->indices[i++] = indices[ii++];
	ri += (count - 3);
	j = i;
	while (i < ri)
	{
		go->indices[j] = go->indices[i - 3];
		go->indices[j + 1] = go->indices[i - 1];
		go->indices[j + 2] = indices[ii++];
		j += 3;
		i++;
	}
	free(indices);
	return (1);
}

unsigned int		parse_indices(t_gameobject *go, t_obj_parser_var *opv, \
	t_parser *parser)
{
	unsigned int	i;
	unsigned int	seed;
	t_seed			*f_seed;
	char			*w;

	i = 0;
	f_seed = &opv->f_seed;
	seed = f_seed->beginseed;
	while (seed < f_seed->endseed && i < f_seed->count && parser->data[seed])
	{
		if ((w = ft_strword(parser->data, &seed)) && ft_strcmp(w, "f") == 0)
		{
			if ((opv->f_count = check_idx_count(parser->data, seed, 0)) < 3)
				return (parser_error(FACE_ERROR, parser->fname, f_seed->line));
			if (!feed_indices(go, get_ret_indices(opv, parser->data, seed), \
				i, opv->f_count))
				return (0);
			i += 3 + ((opv->f_count - 3) * 3);
		}
		seed = skip_line(parser->data, seed);
		if (f_seed->line++ && w)
			free(w);
	}
	go->vtx_count = opv->attrib_fill;
	return (1);
}
