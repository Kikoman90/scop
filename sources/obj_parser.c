/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 20:10:35 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/17 17:16:22 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static t_gameobject	*vtx_feed(t_gameobject *go, char *data, t_seed vtx_seed)
{
	int	i;
	int seed;

	i = 0;
	seed = vtx_seed.beginseed;
	while (seed < vtx_seed.endseed && i < (int)vtx_seed.count && data[seed])
	{
		go->vertices[i] = vec3_atof(data, &seed);
		seed = skip_line(data, seed);
		seed = ft_wordoffset(data, seed);
		i++;
	}
	return (go);
}

static t_gameobject	*idx_feed(t_gameobject *go, char *data, t_seed idx_seed)
{
	int	i;
	int seed;
	int	icount;

	i = 0;
	seed = idx_seed.beginseed;
	while (seed < idx_seed.endseed && i < (int)idx_seed.count && data[seed])
	{
		icount = check_idx_count(data, seed, 1);
		go->indices[i] = ft_atoi_f(ft_strword(data, &seed)) - 1;
		go->indices[i + 1] = ft_atoi_f(ft_strword(data, &seed)) - 1;
		go->indices[i + 2] = ft_atoi_f(ft_strword(data, &seed)) - 1;
		if (icount == 6)
		{
			go->indices[i + 3] = go->indices[i + 2];
			go->indices[i + 4] = go->indices[i + 1];
			go->indices[i + 5] = ft_atoi_f(ft_strword(data, &seed)) - 1;
		}
		seed = skip_line(data, seed);
		seed = ft_wordoffset(data, seed);
		i += icount;
	}
	return (go);
}

static void			*process_go(t_env *env, char *data, t_go_node *node, t_seed *vis)
{	
	node->go = vtx_feed(node->go, data, vis[0]);
	node->go = idx_feed(node->go, data, vis[1]);
	env->go_list = add_go_node(env->go_list, node);
	env->go_count++;
}

//obj_parser.c and obj_feeder.c

t_seed				parse_seed(t_seed seed, )

// split into two functions
void				parse_wavefrontobj(t_env *env, t_parser *parser, int fseed, t_go_node *bound_go)
{
	//t_go_node		*bound_go;
	char			*w;
	char			*name;
	unsigned int	mtl_id;
	t_seed			vi_seed[2];

	//bound_go = NULL;
	name = NULL;
	mtl_id = 0;
	init_seeds(&(vi_seed[0]), &(vi_seed[1]));
	while (seed < parser->fsize && parser->data[seed])
	{
		w = ft_strword(parser->data, &seed);
		if (w && ft_strcmp(w, "o") == 0)
		{
			if (vi_seed[0].count > 0 && vi_seed[1].count > 0)
			{
				bound_go = create_go_node(name, mtl_id, vi_seed[0].count, vi_seed[1].count);
				process_go(env, parser->data, bound_go, vi_seed);
			}
			name = generate_name(ft_strword(parser->data, &seed), GO_NAME, env->go_count);
			init_seeds(&(vi_seed[0]), &(vi_seed[1]));
			mtl_id = 0;
		}
		else if (w && ft_strcmp(w, "v") == 0)
		{
			vi_seed[0] = parse_seed(vi_seed[0], parser->data, seed, VERTEX_SEED);
			vi_seed[0].beginseed = (vi_seed[0].beginseed > -1) ? vi_seed[0].beginseed : seed;
			vi_seed[0].endseed = (vi_seed[0].endseed > seed) ? vi_seed[0].endseed : skip_line(parser->data, seed);
			vi_seed[0].count++;
		}
		else if (w && ft_strcmp(w, "f") == 0)
		{
			vi_seed[1] = parse_seed(vi_seed[1], parser->data, seed, INDEX_SEED);
			vi_seed[1].beginseed = (vi_seed[1].beginseed > -1) ? vi_seed[1].beginseed : seed;
			vi_seed[1].endseed = (vi_seed[1].endseed > seed) ? vi_seed[1].endseed : skip_line(parser->data, seed);
			vi_seed[1].count += check_idx_count(parser->data, seed, 1);
		}
		else if (w && ft_strcmp(w, "mtllib") == 0)
		{
			free(w);
			w = ft_strjoin_rf(parser->fpath, ft_strword(parser->data, &seed));
			//parse_file(env, w, parse_wavefrontmtl);
		}
		else if (w && ft_strcmp(w, "usemtl") == 0)
		{
			//mtl_id = get_mtl_id(ft_strword(parser->data, &seed));
		}
		else if (w && ft_strcmp(w, "#") != 0 && ft_strcmp(w, "s") != 0)
			prefix_error(parser->fname, parser->fline);
		seed = skip_line(parser->data, seed);
		parser->fline++;
		if (w)
			free(w);
	}
	if (vi_seed[0].count > 0 && vi_seed[1].count > 0)
	{
		bound_go = create_go_node(name, mtl_id, vi_seed[0].count, vi_seed[1].count);
		process_go(env, parser->data, bound_go, vi_seed);
	}
}
