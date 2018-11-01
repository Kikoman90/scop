/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtl_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 12:14:23 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/01 21:09:16 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static t_mtl_node	*parse_mtl(t_mtl_list *materials, t_mtl_node *node, \
	t_parser *parser)
{
	char	*mtl_name;

	if (node)
		add_mtl_node(materials, node);
	mtl_name = ft_strword(parser->data, &parser->fseed);
	if (!mtl_name || ft_strlen(mtl_name) == 0)
		mtl_name = ft_strjoin_rf(MTL_NAME, ft_itoa((int)materials->count + 1));
	return (create_mtl_node(mtl_name));
}

static void			parse_attributes(t_mtl_node *node, t_parser *parser, \
	char *word)
{
	if (word && ft_strcmp(word, "Ns") == 0)
		node->mtl->expnt_spc = ft_fclamp(ft_atof_f(ft_strword(parser->data, \
			&parser->fseed)), 0.0f, 100.0f);
	else if (word && ft_strcmp(word, "Ka") == 0)
		node->mtl->clr_amb = vec3_atof(parser->data, &parser->fseed, 1);
	else if (word && ft_strcmp(word, "Kd") == 0)
		node->mtl->clr_dif = vec3_atof(parser->data, &parser->fseed, 1);
	else if (word && ft_strcmp(word, "Ks") == 0)
		node->mtl->clr_spc = vec3_atof(parser->data, &parser->fseed, 1);
	else if (word && ft_strcmp(word, "d") == 0)
		node->mtl->transparency = ft_fclamp(ft_atof_f(ft_strword(parser->data, \
			&parser->fseed)), 0.0f, 1.0f);
	else if (word && ft_strcmp(word, "#") != 0 && ft_strcmp(word, "Ni") != 0 \
		&& ft_strcmp(word, "illum") != 0)
		parser_error(FILE_PREFIX_ERROR, parser->fname, parser->fline);
}

void				parse_wavefrontmtl(t_go_list *gameobjects, \
	t_mtl_list *materials, t_parser *parser, char *word)
{
	t_mtl_node		*node;

	node = NULL;
	(void)gameobjects;
	while (parser->fseed < parser->fsize && parser->data[parser->fseed])
	{
		word = ft_strword(parser->data, &parser->fseed);
		if (word && ft_strcmp(word, "newmtl") == 0)
			node = parse_mtl(materials, node, parser);
		else
			parse_attributes(node, parser, word);
		parser->fseed = skip_line(parser->data, parser->fseed);
		if (parser->fline++ && word)
			free(word);
	}
	if (node)
		add_mtl_node(materials, node);
}
