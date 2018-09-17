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

static int	skip_line(char *data, int seed)
{
	while (data[seed] && data[seed] != '\n')
		seed += 1;
	if (data[seed] && data[seed] == '\n')
		seed += 1;
	return (seed);
}

static void	prefix_error(const char *fname, unsigned int fline)
{
	char	*line_number;
	char	*error_msg;

	line_number = ft_strjoin_lf(ft_strjoin_rf(" (line ", ft_itoa(fline)), ")");
	error_msg = ft_strjoin_bf(ft_strjoin(FILE_PREFIX_ERROR, "'"), \
			ft_strjoin_bf(ft_strjoin(fname, "'"), line_number));
	log_error_free(error_msg);
}

int			check_idx_count(char *data, int seed)
{
	int count;

	count = 0;
	while (data[seed] != '\n' && data[seed] != '\0' && \
		(seed = ft_wordoffset(data, seed)) > -1)
		count++;
	if (count == 4)
		count = 6;
	return (count);
}

t_go_node	*process_go(t_env *env, char *data, t_go_node *node, t_seed *vis)
{
	int	i;
	int icount;
	int seed;

	node->go->vtx_count = vis[0].count;
	node->go->idx_count = vis[1].count;
	if (!(node->go->vertices = (t_vec3*)malloc(sizeof(t_vec3) * vis[0].count)) || \
	!(node->go->indices = (unsigned int*)malloc(sizeof(unsigned int) * vis[1].count)))
	{
		clean_go_node(node);
		return (log_error_null(MALLOC_ERROR));
	}
	i = 0;
	seed = vis[0].beginseed;
	while (seed < vis[0].endseed && i < vis[0].count && data[seed])
	{
		node->go->vertices[i].x = ft_atof_f(ft_strword(data, &seed));
		node->go->vertices[i].y = ft_atof_f(ft_strword(data, &seed));
		node->go->vertices[i].z = ft_atof_f(ft_strword(data, &seed));
		seed = skip_line(data, seed);
		seed = ft_wordoffset(data, seed);
		i++;
	}
	i = 0;
	seed = vis[1].beginseed;
	while (seed < vis[1].endseed && i < vis[1].count && data[seed])
	{
		icount = check_idx_count(data, seed);
		node->go->indices[i] = ft_atoi_f(ft_strword(data, &seed)) - 1;
		node->go->indices[i + 1] = ft_atoi_f(ft_strword(data, &seed)) - 1;
		node->go->indices[i + 2] = ft_atoi_f(ft_strword(data, &seed)) - 1;
		if (icount == 6)
		{
			node->go->indices[i + 3] = node->go->indices[i + 2];
			node->go->indices[i + 4] = node->go->indices[i + 1];
			node->go->indices[i + 5] = ft_atoi_f(ft_strword(data, &seed)) - 1;
		}
		seed = skip_line(data, seed);
		seed = ft_wordoffset(data, seed);
		i += icount;
	}
	env->obj_list = add_go_node(env, node);
	return (node);
}

static void	parse_wavefrontobj(t_env *env, int seed)
{
	char			*w;
	t_go_node		*bound_go;
	t_seed			vi_seed[2];

	bound_go = NULL;
	init_seeds(&(vi_seed[0]), &(vi_seed[1]));
	while (seed < env->parser->fsize && env->parser->data[seed])
	{
		w = ft_strword(env->parser->data, &seed);
		if (ft_strcmp(w, "o") == 0)
		{
			if (vi_seed[0].count > 0 && vi_seed[1].count > 0 && vi_seed[1].count % 3 == 0)
			{
				if (!bound_go)
					bound_go = create_go_node(generate_go_name(env->obj_count));
				process_go(env, env->parser->data, bound_go, vi_seed);
			}
			bound_go = create_go_node(ft_strword(env->parser->data, &seed));
			init_seeds(&(vi_seed[0]), &(vi_seed[1]));
		}
		else if (ft_strcmp(w, "v") == 0)
		{
			vi_seed[0].beginseed = (vi_seed[0].beginseed > -1) ? vi_seed[0].beginseed : seed;
			vi_seed[0].endseed = (vi_seed[0].endseed > seed) ? vi_seed[0].endseed : skip_line(env->parser->data, seed);
			vi_seed[0].count++;
		}
		else if (ft_strcmp(w, "f") == 0)
		{
			vi_seed[1].beginseed = (vi_seed[1].beginseed > -1) ? vi_seed[1].beginseed : seed;
			vi_seed[1].endseed = (vi_seed[1].endseed > seed) ? vi_seed[1].endseed : skip_line(env->parser->data, seed);
			vi_seed[1].count += check_idx_count(env->parser->data, seed);
		}
		else if (ft_strcmp(w, "mtllib") == 0)
		{
			ft_putendl("MTLLIB");
			//load Mtlfile
		}
		else if (ft_strcmp(w, "usemtl") == 0)
		{
			ft_putendl("USEMTL");
			//fetch mtl
		}
		else if (ft_strcmp(w, "#") != 0 && ft_strcmp(w, "s") != 0)
			prefix_error(env->parser->fname, env->parser->fline);
		seed = skip_line(env->parser->data, seed);
		env->parser->fline++;
		if (w)
			free(w);
	}
	if (vi_seed[0].count > 0 && vi_seed[1].count > 0 && vi_seed[1].count % 3 == 0)
	{
		if (!bound_go)
			bound_go = create_go_node(generate_go_name(env->obj_count));
		process_go(env, env->parser->data, bound_go, vi_seed);
	}
}

void		parse_file(t_env *env, const char *path)
{
	int	fd;

	if (init_parser(env->parser, path, &fd) != -1)
	{
		if ((env->parser->data = (char *)mmap(NULL, env->parser->fsize, \
			PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			log_error_free(ft_strjoin("(mmap) ", strerror(errno)));
		else
		{
			env->parser->data[env->parser->fsize] = '\0';
			parse_wavefrontobj(env, 0);
			if (munmap(env->parser->data, env->parser->fsize) == -1)
				log_error_free(ft_strjoin("(munmap) ", strerror(errno)));
		}
		close(fd);
	}
}
