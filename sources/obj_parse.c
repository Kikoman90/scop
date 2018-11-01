/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 20:10:35 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/01 22:04:09 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	init_gl_objects(t_gameobject *go, size_t buf_s, size_t attr_s)
{
	glGenVertexArrays(1, &go->gl_stack.vao);
	glBindVertexArray(go->gl_stack.vao);
	glGenBuffers(1, &go->gl_stack.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, go->gl_stack.vbo);
	glBufferData(GL_ARRAY_BUFFER, buf_s * go->vtx_count, &go->vtx_attrib[0], \
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, buf_s, (void*)(2 * attr_s));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, buf_s, (void*)(5 * attr_s));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, buf_s, 0);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, buf_s, (void*)(8 * attr_s));
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &go->gl_stack.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->gl_stack.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * go->idx_count,\
		&go->indices[0], GL_STATIC_DRAW);
}

static void	parse_go(t_go_list *gameobjects, t_parser *parser, \
	t_obj_parser_var *opv)
{
	t_go_node		*node;

	if (opv->f_seed.count > 0 && opv->v_seed[0].count > 0)
	{
		if (!opv->name || ft_strlen(opv->name) == 0)
			opv->name = ft_strjoin_rf(GO_NAME, ft_itoa(gameobjects->count + 1));
		if ((node = create_go_node(opv->name, opv->mtl_id, opv->f_seed.count)))
		{
			if (!parse_indices(node->go, opv, parser))
				clean_go_node(node, 1);
			if (!(node->go->vtx_attrib = (t_vtx_attrib*)malloc(\
				sizeof(t_vtx_attrib) * node->go->vtx_count)))
			{
				log_error(MALLOC_ERROR);
				clean_go_node(node, 1);
			}
			parse_vtx_attrib(node->go, opv, parser->data);
			init_gl_objects(node->go, sizeof(t_vtx_attrib), sizeof(float));
			add_go_node(gameobjects, node);
		}
	}
	else if (opv->name)
		free(opv->name);
	opv->attrib_list = free_attrib_list(opv->attrib_list);
	init_opv(opv, ft_strword(parser->data, &parser->fseed), opv->mtl_offset);
}

static void	parse_attrib(t_parser *parser, t_obj_parser_var *opv, char *word, \
	int f_attrib)
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

static void	parse_mtllib(t_go_list *gameobjects, t_mtl_list *materials, \
	t_parser *parser)
{
	char	*path;

	path = ft_strjoin_rf(parser->fpath, \
		ft_strword(parser->data, &parser->fseed));
	parse_file(gameobjects, materials, path, parse_wavefrontmtl);
	free(path);
}

void		parse_wavefrontobj(t_go_list *gameobjects, \
	t_mtl_list *materials, t_parser *parser, char *word)
{
	t_obj_parser_var	opv;

	init_opv(&opv, NULL, materials->count);
	while (parser->fseed < parser->fsize && parser->data[parser->fseed])
	{
		word = ft_strword(parser->data, &parser->fseed);
		if (word && ft_strcmp(word, "o") == 0)
			parse_go(gameobjects, parser, &opv);
		else if (word && word[0] && word[0] == 'v')
			parse_attrib(parser, &opv, word, 0);
		else if (word && ft_strcmp(word, "f") == 0)
			parse_attrib(parser, &opv, word, 1);
		else if (word && ft_strcmp(word, "mtllib") == 0)
			parse_mtllib(gameobjects, materials, parser);
		else if (word && ft_strcmp(word, "usemtl") == 0)
			opv.mtl_id = get_mtl_id(materials->head, \
				ft_strword(parser->data, &parser->fseed), opv.mtl_offset);
		else if (word && ft_strcmp(word, "#") != 0 && ft_strcmp(word, "s") != 0)
			parser_error(FILE_PREFIX_ERROR, parser->fname, parser->fline);
		parser->fseed = skip_line(parser->data, parser->fseed);
		if (parser->fline++ && word)
			free(word);
	}
	parse_go(gameobjects, parser, &opv);
	opv.attrib_list = free_attrib_list(opv.attrib_list);
}
