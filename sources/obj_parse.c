/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 20:10:35 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/29 19:32:02 by fsidler          ###   ########.fr       */
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
	while (i <= idx && i <= v_seed->count && seed < v_seed->endseed && data[seed])
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

void			parse_vtx_attrib(t_gameobject *go, t_obj_parser_var *opv, \
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
			go->vtx_attrib[i].color = vec3_xyz(
				sinf( go->vtx_attrib[i].vertex.x ), \
				sinf( go->vtx_attrib[i].vertex.y ), \
				sinf( go->vtx_attrib[i].vertex.z ));
			i++;
		}
		tmp = tmp->next;
	}
}

/*void			print_attrib_list(t_idx_attrib *list)
{
	t_idx_attrib	*tmp;

	tmp = list;
	printf("=----- current list -----=\n");
	while (tmp)
	{
		printf("idx_attrib->attrib[3] = {%u, %u, %u}\n", tmp->attrib[0], \
			tmp->attrib[1], tmp->attrib[2]);
		printf("idx_attrib->idx_ret = %u\n", tmp->idx_ret);
		tmp = tmp->next;
	}
	printf("=------------------------=\n");
}*/

unsigned int	cmp_attrib(t_idx_attrib *att1, t_idx_attrib *att2, \
	unsigned int *ret)
{
	if (att1->attrib[0] == att2->attrib[0] && \
		att1->attrib[1] == att2->attrib[1] && \
		att1->attrib[2] == att2->attrib[2])
	{
		*ret = att1->idx_ret;
		return (1);
	}
	return (0);
}

t_idx_attrib	*check_attrib_list(t_obj_parser_var *opv, t_idx_attrib *elem, \
	unsigned int *ret)
{
	t_idx_attrib	*tmp;
	t_idx_attrib	*attrib_ret;

	tmp = opv->attrib_list;
	while (tmp && tmp->next)
	{
		if (cmp_attrib(tmp, elem, ret) == 1)
			return (tmp);
		tmp = tmp->next;
	}
	if (tmp && cmp_attrib(tmp, elem, ret) == 1)
		return (tmp);
	if (!(attrib_ret = (t_idx_attrib*)malloc(sizeof(t_idx_attrib))))
		return (log_error_null(MALLOC_ERROR));
	attrib_ret->idx_ret = opv->attrib_fill++;
	attrib_ret->attrib[0] = elem->attrib[0];
	attrib_ret->attrib[1] = elem->attrib[1];
	attrib_ret->attrib[2] = elem->attrib[2];
	attrib_ret->next = NULL;
	*ret = attrib_ret->idx_ret;
	if (tmp)
		tmp->next = attrib_ret;
	else
		opv->attrib_list = attrib_ret;
	return (attrib_ret);
}

unsigned int	*get_ret_indices(t_obj_parser_var *opv, char *data, \
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

static unsigned int		feed_indices(t_gameobject *go, unsigned int *indices, \
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

unsigned int	parse_indices(t_gameobject *go, t_obj_parser_var *opv, \
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

static void		parse_go(t_env *env, t_parser *parser, t_obj_parser_var *opv, \
					unsigned int free_opv)
{
	t_go_node		*node;

	if (opv->f_seed.count > 0 && opv->f_seed.count % 3 == 0 && \
		opv->v_seed[0].count > 0)
	{
		if (!opv->name || ft_strlen(opv->name) == 0)
			opv->name = ft_strjoin_rf(GO_NAME, ft_itoa((int)env->go_count + 1));
		if ((node = create_go_node(opv->name, opv->mtl_id, opv->f_seed.count)))
		{
			if (!parse_indices(node->go, opv, parser) || !(node->go->vtx_attrib\
				= (t_vtx_attrib*)malloc(sizeof(t_vtx_attrib) * node->go->vtx_count)))
				clean_go_node(node, 0);
			parse_vtx_attrib(node->go, opv, parser->data);
			init_gl_objects(node->go, (char*)NULL, sizeof(float));
			env->go_list = add_go_node(env, node);
		}
	}
	if (free_opv == 1)
	{
		opv->attrib_list = free_attrib(opv->attrib_list);
		free(opv);
	}
	else
		opv = init_opv(opv, ft_strword(parser->data, &parser->fseed), \
			opv->mtl_offset);
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
