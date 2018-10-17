/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtl_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 12:14:23 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/17 17:13:04 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static t_mtl_node	*parse_mtl(t_env *env, char *word, t_mtl_node *node)
{
	char	*mtl_name;

	if (node)
		env->mtl_list = add_mtl_node(env, node);
	if (word && ft_strlen(word) > 0)
		mtl_name = word;
	else
		mtl_name = ft_strjoin_rf(MTL_NAME, ft_itoa((int)env->mtl_count + 1));
	return (create_mtl_node(mtl_name));
}

void				parse_wavefrontmtl(t_env *env, t_parser *parser, char *word)
{
	t_mtl_node		*bound_mtl;

	bound_mtl = NULL;
	while (parser->fseed < parser->fsize && parser->data[parser->fseed])
	{
		word = ft_strword(parser->data, &parser->fseed);
		if (word && ft_strcmp(word, "newmtl") == 0)
			bound_mtl = parse_mtl(env, ft_strword(parser->data, &parser->fseed), bound_mtl);
		else if (word && ft_strcmp(word, "Ns") == 0)
			bound_mtl->mtl->expnt_spc = ft_fclamp(ft_atof_f(ft_strword(parser->data, &parser->fseed)), 0, 100);
		else if (word && ft_strcmp(word, "Ka") == 0)
			bound_mtl->mtl->clr_amb = vec3_atof(parser->data, &parser->fseed, 1);
		else if (word && ft_strcmp(word, "Kd") == 0)
			bound_mtl->mtl->clr_dif = vec3_atof(parser->data, &parser->fseed, 1);
		else if (word && ft_strcmp(word, "Ks") == 0)
			bound_mtl->mtl->clr_spc = vec3_atof(parser->data, &parser->fseed, 1);
		else if (word && ft_strcmp(word, "d") == 0)
			bound_mtl->mtl->transparency = ft_fclamp(ft_atof_f(ft_strword(parser->data, &parser->fseed)), 0, 1);
		else if (word && ft_strcmp(word, "#") != 0 && ft_strcmp(word, "Ni") != 0 && ft_strcmp(word, "illum") != 0)
			parser_error(FILE_PREFIX_ERROR, parser->fname, parser->fline);
		parser->fseed = skip_line(parser->data, parser->fseed);
		if (parser->fline++ && word)
			free(word);
	}
	if (bound_mtl)
		env->mtl_list = add_mtl_node(env, bound_mtl);
}
