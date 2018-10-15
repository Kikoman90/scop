/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtl_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 12:14:23 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/12 17:12:38 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void		parse_wavefrontmtl(t_env *env, t_parser *parser, char *word)
{
	t_mtl_node		*bound_mtl;

	bound_mtl = NULL;
	while (parser->fseed < parser->fsize && parser->data[parser->fseed])
	{
		word = ft_strword(parser->data, &parser->fseed);
		if (word && ft_strcmp(word, "newmtl") == 0)
		{
			if (bound_mtl)
				env->mtl_list = add_mtl_node(env, bound_mtl);
			bound_mtl = create_mtl_node(generate_name(ft_strword(parser->data, &parser->fseed), MTL_NAME, env->mtl_count));
		}
		else if (word && ft_strcmp(word, "Ns") == 0)
			bound_mtl->mtl->expnt_spc = fclamp(ft_atof_f(ft_strword(parser->data, &parser->fseed)), 0, 100);
		else if (word && ft_strcmp(word, "Ka") == 0)
			bound_mtl->mtl->clr_amb = vec3_atof(parser->data, &parser->fseed, 1);
		else if (word && ft_strcmp(word, "Kd") == 0)
			bound_mtl->mtl->clr_dif = vec3_atof(parser->data, &parser->fseed, 1);
		else if (word && ft_strcmp(word, "Ks") == 0)
			bound_mtl->mtl->clr_spc = vec3_atof(parser->data, &parser->fseed, 1);
		else if (word && ft_strcmp(word, "d") == 0)
			bound_mtl->mtl->transparency = fclamp(ft_atof_f(ft_strword(parser->data, &parser->fseed)), 0, 1);
		else if (word && ft_strcmp(word, "#") != 0 && ft_strcmp(word, "Ni") != 0 && ft_strcmp(word, "illum") != 0)
			parser_error(FILE_PREFIX_ERROR, parser->fname, parser->fline);
		parser->fseed = skip_line(parser->data, parser->fseed);
		parser->fline++;
		if (word)
			free(word);
	}
	if (bound_mtl)
		env->mtl_list = add_mtl_node(env, bound_mtl);
}
