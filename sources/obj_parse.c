/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 20:10:35 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/24 17:43:13 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/*int				valid_idx_check(unsigned int *idx, unsigned int vtx_count, \
					unsigned int idx_count)
{
	if (*idx > vtx_count || *(idx + 1) > vtx_count || *(idx + 2) > vtx_count)
		return (0);
	if (idx_count == 6 && *(idx + 5) > vtx_count)
		return (0);
	return (1);
}

// 1 free argument (= 28 lines) -> 3 more to go.
static int		idx_feed(t_gameobject *go, char *data, t_seed *idx_seed)
{
	unsigned int	i;
	unsigned int	seed;
	unsigned int	idx_count;
	char			*word;

	i = 0;
	seed = idx_seed->beginseed;
	while (seed < idx_seed->endseed && i < idx_seed->count && data[seed])
	{
		if ((word = ft_strword(data, &seed)) && ft_strcmp(word, "f") == 0)
		{
			idx_count = check_idx_count(data, seed, 1);
			go->indices[i] = ft_atoi_f(ft_strword(data, &seed)) - 1;
			go->indices[i + 1] = ft_atoi_f(ft_strword(data, &seed)) - 1;
			go->indices[i + 2] = ft_atoi_f(ft_strword(data, &seed)) - 1;
			if (idx_count == 6)
			{
				go->indices[i + 3] = go->indices[i];
				go->indices[i + 4] = go->indices[i + 2];
				go->indices[i + 5] = ft_atoi_f(ft_strword(data, &seed)) - 1;
			}
			if (!valid_idx_check(&go->indices[i], go->vtx_count - 1, idx_count))
				return (0);
			free(word);
			i += idx_count;
		}
		seed = skip_line(data, seed);
		idx_seed->line += 1;
	}
	return (1);
}

static int		vtx_idx_feed(t_gameobject *go, char *data, const char *fname, \
					t_obj_parser_var *opv)
{
	unsigned int	i;
	unsigned int	seed;
	char			*word;
	t_seed			*vtx_seed;

	i = 0;
	vtx_seed = &opv->vtx_seed;
	seed = vtx_seed->beginseed;
	while (seed < vtx_seed->endseed && i < vtx_seed->count && data[seed])
	{
		if ((word = ft_strword(data, &seed)) && ft_strcmp(word, "v") == 0)
		{
			go->vertices[i++] = vec3_atof(data, &seed, 0);
			free(word);
		}
		seed = skip_line(data, seed);
		vtx_seed->line += 1;
	}
	if (!(idx_feed(go, data, &opv->idx_seed)))
	{
		parser_error(INVALID_IDX_ERROR, fname, opv->idx_seed.line);
		return (0);
	}
	return (1);
}

static void		trim_vtx_attrib(char *word, char *data, unsigned int seed, unsigned int **v_idx)
{
	char	*idx_w;

	if (word)
		free(word);
	word = ft_strword(data, &seed);
	idx_w = word;
	v_idx[0] = ft_atoi_f(idx_w);
	idx_w = ft_strchr(idx_w, '/');
	v_idx[1] = ft_atoi_f(idx_w);
	idx_w = ft_strchr(idx_w, '/');
	v_idx[2] = ft_atoi_f(idx_w);
	free(word);
	word = NULL;
}*/

/*----------------------------------------------------------------------------*/



static t_vec3		vtx_attrib_feed(t_seed *v_seed, char *data, \
	unsigned int idx, char *v);
{
	unsigned int	i;
	unsigned int	seed;
	char			*word;

	i = 0;
	idx -= (idx > 0) ? 1 : 0;
	seed = v_seed->beginseed;
	while (i < idx && i < v_seed->count && seed < v_seed->endseed && data[seed])
	{
		if ((word = ft_strword(data, &seed)) && ft_strcmp(word, v) == 0)
		{
			free(word);
			if (i == idx)
				return (vec3_atof(data, &seed, 0));
			i++;
		}
		seed = skip_line(data, seed);
		v_seed->line++;
	}
	return ((t_vec3)VEC3_ZERO);
}

static unsigned int	vtx_feed(t_gameobject *go, t_obj_parser_var *opv, \
	char *data, unsigned int *attrib)
{
	static unsigned int	ret = 0;

	go->vtx_attrib[ret].vertex = vtx_attrib_feed(&opv->v_seed[0], data, \
		attrib[0], "v");
	go->vtx_attrib[ret].uv = vec2_v3(vtx_attrib_feed(&opv->v_seed[1], data, \
		attrib[1], "vt"));
	go->vtx_attrib[ret].normal = vec3_norm(vtx_attrib_feed(&opv->v_seed[2], \
		data, attrib[2], "vn"));
	//go->vtx_attrib[ret].color = gen_color(go->vtx_attrib[ret].vertex); //worldspace function
	return (ret);
}

static unsigned int	*vtx_attrib(t_gameobject *go, t_obj_parser_var *opv, \
	char *data, unsigned int seed)
{
	unsigned int	i;
	unsigned int	*ret;
	char			*word;
	t_idx_attrib	*f_tmp;

	i = 0;
	if (!(ret = (unsigned int*)malloc(sizeof(unsigned int) * opv->f_count)) || \
		!(f_tmp = (t_idx_attrib*)malloc(sizeof(t_idx_attrib))))
			return (log_error_null(MALLOC_ERROR));
	while (i < opv->f_count)
	{
		word = ft_strword(data, &seed);
		f_tmp->attrib[0] = ft_atoi(word);
		f_tmp->attrib[1] = ft_atoi(ft_strchr(word, '/', 1));
		f_tmp->attrib[2] = ft_atoi(ft_strnchr(word, '/', 2, 1));
		if (word)
			free(word);
		if (!cmp_attrib_list(opv->attrib_list, f_tmp))
			f_tmp->idx_ret = vtx_feed(go, opv, data, f_tmp->attrib);
		ret[i] = f_tmp->idx_ret;
		i++;
	}
	free(f_tmp);
	return (ret);
}

static void		idx_feed(t_gameobject *go, unsigned int *indices, \
	unsigned int i, unsigned int count)
{
	unsigned int	j;
	unsigned int	ri;
	unsigned int	ii;
	
	ii = 0;
	ri = i + 3;
	if (!indices)
		return (0);
	while (i < ri)
		go->indices[i++] = indices[ii++];
	ri += count - 3;
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

static int		attrib_feed(t_obj_parser_var *opv, t_parser *psr, \
	t_gameobject *go, char *word)
{
	unsigned int	i;
	unsigned int	seed;
	t_seed			*f_seed;
	
	i = 0;
	f_seed = &opv->f_seed;
	seed = f_seed->beginseed;
	while (seed < f_seed->endseed && i < f_seed->count && psr->data[seed])
	{
		if ((word = ft_strword(psr->data, &seed)) && ft_strcmp(word, "f") == 0)
		{
			if ((opv->f_count = check_idx_count(psr->data, seed, 0)) < 3)
				return (parser_error(FACE_ERROR, psr->fname, f_seed->line));
			if (!idx_feed(go, vtx_attrib(opv, psr->data, seed)), \
				i, opv->f_count))
				return (0);
			i += 3 + ((opv->f_count - 3) * 3);
		}
		seed = skip_line(psr->data, seed);
		if (f_seed->line++ && word)
			free(word);
	}
	return (1);
}

static void		parse_attrib(t_obj_parser_var *opv, t_parser *parser, \
	char *word, int f_attrib)
{
	t_seed	*seed;

	seed = NULL;
	if (f_attrib)
		seed = &opv->f_seed;
	else if (ft_strcmp(word, "v") == 0)
		seed = &opv->v_seed[0];
	else if (ft_strcmp(word, "vt") == 0)
		seed = &opv->v_seed[1];
	else if (ft_strcmp(word, "vn") == 0)
		seed = &opv->v_seed[2];
	else
		return ;
	if (seed && seed->beginseed == 0 && seed->endseed == 0)
	{
		seed->beginseed = parser->fseed - ft_strlen(word);
		seed->line = parser->fline;
	}
	if (seed && seed->endseed <= parser->fseed)
		seed->endseed = skip_line(parser->data, parser->fseed);
	if (seed && f_attrib == 1)
		seed->count += check_idx_count(parser->data, parser->fseed, 1);
	else
		seed->count++;
}

char			*parse_mtllib(t_env *env, t_parser *parser, char *word)
{
	if (word)
	{
		free(word);
		word = NULL;
	}
	word = ft_strjoin_rf(parser->fpath, ft_strword(parser->data, \
							&parser->fseed));
	parse_file(env, word, parse_wavefrontmtl);
	free(word);
	return (NULL);
}

static void		parse_go(t_env *env, t_parser *parser, t_obj_parser_var *opv, \
					unsigned int free_opv)
{
	t_go_node	*node;

	node = NULL;
	if (opv->f_seed.count > 0 && opv->f_seed.count % 3 == 0 && \
		opv->v_seed[0].count > 0)
	{
		if (!opv->name || ft_strlen(opv->name) == 0)
			opv->name = ft_strjoin_rf(GO_NAME, ft_itoa((int)env->go_count + 1));
		if ((node = create_go_node(opv->name, opv->mtl_id, \
			opv->v_seed[0].count, opv->f_seed.count)))
		{
			if (!attrib_feed(opv, parser, node->go, NULL))
				clean_go_node(node, 0);
			else
			{
				init_gl_objects(node->go);
				env->go_list = add_go_node(env, node);
			}
		}
	}
	if (free_opv == 1)
		free(opv);
	else
		opv = init_opv(opv, ft_strword(parser->data, &parser->fseed), \
			opv->mtl_offset);
}

void			parse_wavefrontobj(t_env *env, t_parser *parser, char *word)
{
	t_obj_parser_var	*opv;

	if (!(opv = init_opv(NULL, NULL, env->mtl_count)))
		return ;
	while (parser->fseed < parser->fsize && parser->data[parser->fseed])
	{
		word = ft_strword(parser->data, &parser->fseed);
		if (word && ft_strcmp(word, "o") == 0)
			parse_go(env, parser, opv, 0);
		else if (word && word[0] && word[0] == 'v')
			parse_attrib(opv, parser, word, 0);
		else if (word && ft_strcmp(word, "f") == 0)
			parse_attrib(opv, parser, word, 1);
		else if (word && ft_strcmp(word, "mtllib") == 0)
			word = parse_mtllib(env, parser, word);
		else if (word && ft_strcmp(word, "usemtl") == 0)
			opv->mtl_id = get_mtl_id(env, ft_strword(parser->data, \
				&parser->fseed), opv->mtl_offset);
		else if (word && ft_strcmp(word, "#") != 0 && ft_strcmp(word, "s") != 0)
			parser_error(FILE_PREFIX_ERROR, parser->fname, parser->fline);
		parser->fseed = skip_line(parser->data, parser->fseed);
		if (parser->fline++ && word)
			free(word);
	}
	parse_go(env, parser, opv, 1);
}
