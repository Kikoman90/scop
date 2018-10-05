/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 20:10:35 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/04 20:26:35 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static t_gameobject	*vtx_feed(t_gameobject *go, char *data, t_seed vtx_seed)
{
	unsigned int	i;
	unsigned int	seed;

	i = 0;
	seed = vtx_seed.beginseed;
	while (seed < vtx_seed.endseed && i < vtx_seed.count && data[seed])
	{
		go->vertices[i] = vec3_atof(data, &seed, 0);
		seed = skip_line(data, seed);
		seed = ft_wordoffset(data, seed);
		i++;
	}
	return (go);
}

static t_gameobject	*idx_feed(t_gameobject *go, char *data, t_seed idx_seed)
{
	unsigned int	i;
	unsigned int	seed;
	unsigned int	idx_count;

	i = 0;
	seed = idx_seed.beginseed;
	while (seed < idx_seed.endseed && i < idx_seed.count && data[seed])
	{
		idx_count = check_idx_count(data, seed, 1);
		go->indices[i] = iclamp(ft_atoi_f(ft_strword(data, &seed)) - 1, 0, idx_seed.count - 1);
		go->indices[i + 1] = iclamp(ft_atoi_f(ft_strword(data, &seed)) - 1, 0, idx_seed.count - 1);
		go->indices[i + 2] = iclamp(ft_atoi_f(ft_strword(data, &seed)) - 1, 0, idx_seed.count - 1);
		if (idx_count == 6)
		{
			go->indices[i + 3] = go->indices[i + 2];
			go->indices[i + 4] = go->indices[i + 1];
			go->indices[i + 5] = iclamp(ft_atoi_f(ft_strword(data, &seed)) - 1, 0, idx_seed.count - 1);
		}
		seed = skip_line(data, seed);
		seed = ft_wordoffset(data, seed);
		i += idx_count;
	}
	return (go);
}

t_seed				parse_attr(t_seed seed, char *data, \
								unsigned int fseed, int idx)
{
	if (seed.beginseed == 0)
		seed.beginseed = fseed;
	if (seed.endseed <= fseed)
		seed.endseed = skip_line(data, fseed);
	if (idx == 1)
		seed.count += check_idx_count(data, fseed, 1);
	else
		seed.count++;
	return (seed);
}

char				*parse_mtllib(t_env *env, t_parser *parser, char *word)
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
	word = NULL;
	return (word);
}

void				parse_go(t_env *env, t_parser *parser, \
								t_obj_parser_var *opv, t_go_node *node)
{
	if (opv->idx_seed.count > 0 && opv->idx_seed.count % 3 == 0 && \
		opv->vtx_seed.count > 0)
	{
		opv->name = generate_name(opv->name, GO_NAME, env->go_count);
		node = create_go_node(opv->name, opv->mtl_id, \
								opv->vtx_seed.count, opv->idx_seed.count);
		node->go = vtx_feed(node->go, parser->data, opv->vtx_seed);
		node->go = idx_feed(node->go, parser->data, opv->idx_seed);
		gl_stack_feed(node->go);
		env->go_list = add_go_node(env, node);
	}
	if (parser->fseed < parser->fsize && parser->data[parser->fseed])
		opv = init_obj_parser_var(opv, ft_strword(parser->data, \
									&parser->fseed), opv->mtl_offset);
}

void				parse_wavefrontobj(t_env *env, t_parser *parser, char *word)
{
	t_obj_parser_var	*opv;

	if (!(opv = init_obj_parser_var(NULL, NULL, env->mtl_count)))
		return ;
	while (parser->fseed < parser->fsize && parser->data[parser->fseed])
	{
		word = ft_strword(parser->data, &parser->fseed);
		if (word && ft_strcmp(word, "o") == 0)
			parse_go(env, parser, opv, NULL);
		else if (word && ft_strcmp(word, "v") == 0)
			opv->vtx_seed = parse_attr(opv->vtx_seed, parser->data, parser->fseed, 0);
		else if (word && ft_strcmp(word, "f") == 0)
			opv->idx_seed = parse_attr(opv->idx_seed, parser->data, parser->fseed, 1);
		else if (word && ft_strcmp(word, "mtllib") == 0)
			word = parse_mtllib(env, parser, word);
		else if (word && ft_strcmp(word, "usemtl") == 0)
			opv->mtl_id = get_mtl_id(env, ft_strword(parser->data, &parser->fseed), opv->mtl_offset);
		else if (word && ft_strcmp(word, "#") != 0 && ft_strcmp(word, "s") != 0)
			prefix_error(parser->fname, parser->fline);
		parser->fseed = skip_line(parser->data, parser->fseed);
		parser->fline++;
		if (word)
			free(word);
	}
	parse_go(env, parser, opv, NULL);
	free(opv);
}
