/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 20:10:35 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/22 11:35:47 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

int				valid_idx_check(unsigned int *idx, unsigned int vtx_count, \
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

static t_seed	parse_attr(t_seed seed, t_parser *parser, int idx)
{
	if (seed.beginseed == 0 && seed.endseed == 0)
	{
		seed.beginseed = parser->fseed - 1;
		seed.line = parser->fline;
	}
	if (seed.endseed <= parser->fseed)
		seed.endseed = skip_line(parser->data, parser->fseed);
	if (idx == 1)
		seed.count += check_idx_count(parser->data, parser->fseed, 1);
	else
		seed.count++;
	return (seed);
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
	word = NULL;
	return (word);
}

static void		parse_go(t_env *env, t_parser *parser, t_obj_parser_var *opv, \
					unsigned int free_opv)
{
	t_go_node	*node;

	node = NULL;
	if (opv->idx_seed.count > 0 && opv->idx_seed.count % 3 == 0 && \
		opv->vtx_seed.count > 0)
	{
		if (!opv->name || ft_strlen(opv->name) == 0)
			opv->name = ft_strjoin_rf(GO_NAME, ft_itoa((int)env->go_count + 1));
		if ((node = create_go_node(opv->name, opv->mtl_id, \
				opv->vtx_seed.count, opv->idx_seed.count)))
		{
			if (!vtx_idx_feed(node->go, parser->data, parser->fname, opv))
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
		else if (word && ft_strcmp(word, "v") == 0)
			opv->vtx_seed = parse_attr(opv->vtx_seed, parser, 0);
		else if (word && ft_strcmp(word, "f") == 0)
			opv->idx_seed = parse_attr(opv->idx_seed, parser, 1);
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
