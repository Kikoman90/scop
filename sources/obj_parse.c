/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 20:10:35 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/31 13:08:28 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

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
			init_gl_objects(node->go, sizeof(t_vtx_attrib), sizeof(float));
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
