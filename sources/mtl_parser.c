/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtl_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 12:14:23 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/18 12:14:24 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

// split into two functions
void        parse_wavefrontmtl(t_env *env, t_parser *parser, char *word)
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
	}
	/*while (seed < parser->fsize && parser->data[seed])
	{
		w = ft_strword(parser->data, &seed);
		//here
		if (w && ft_strcmp(w, "newmtl") == 0)
		{
			if (vi_seed[0].count > 0 && vi_seed[1].count > 0)
			{
				if (!bound_go)
					bound_go = create_go_node(generate_go_name(env->obj_count));
				process_go(env, parser->data, bound_go, vi_seed);
			}
			bound_go = create_go_node(ft_strword(parser->data, &seed));
			init_seeds(&(vi_seed[0]), &(vi_seed[1]));
		}
		else if (w && ft_strcmp(w, "Ka") == 0)
		{

		}
		else if (w && ft_strcmp(w, "f") == 0)
		{
			vi_seed[1].beginseed = (vi_seed[1].beginseed > -1) ? vi_seed[1].beginseed : seed;
			vi_seed[1].endseed = (vi_seed[1].endseed > seed) ? vi_seed[1].endseed : skip_line(parser->data, seed);
			vi_seed[1].count += check_idx_count(parser->data, seed, 1);
		}
		else if (w && ft_strcmp(w, "mtllib") == 0)
		{
			free(w);
			w = ft_strjoin(parser->fpath, ft_strword(parser->data, &seed));
			parse_file(env, w, parse_wavefrontobj);
			free(w);
		}
		else if (w && ft_strcmp(w, "usemtl") == 0)
		{
			ft_putendl("USEMTL");
			//fetch mtl
		}
		else if (w && ft_strcmp(w, "#") != 0)
			prefix_error(parser->fname, parser->fline);
		seed = skip_line(parser->data, seed);
		parser->fline++;
		if (w)
			free(w);
	}
	if (vi_seed[0].count > 0 && vi_seed[1].count > 0)
	{
		if (!bound_go)
			bound_go = create_go_node(generate_go_name(env->obj_count));
		process_go(env, parser->data, bound_go, vi_seed);
	}*/
}