/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 19:17:55 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 19:42:08 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

unsigned int		init_light(t_light *light, t_vec3 pos, float intensity, \
	float range)
{
	glActiveTexture(GL_TEXTURE0);
	if (!(light->texture_id = \
		create_texture("resources/gizmos/light.tga", get_tga_texture)))
		return (0);
	light->transform = init_transform_trs(pos, quat(), vec3_f(0.35f));
	light->color[0] = vec3_xyz(1.0f, 0.62f, 0.45f);
	light->color[1] = vec3_xyz(0.5f, 0.9f, 0.67f);
	light->color[2] = vec3_xyz(0.6f, 0.6f, 0.6f);
	light->color[3] = vec3_xyz(0.3f, 0.6f, 0.92f);
	light->intensity = intensity;
	light->range = range;
	light->id = 1;
	light->pick_clr = generate_pick_clr(light->id);
	return (1);
}

static unsigned int	get_geometry(t_geometry *primitive, t_go_list *list)
{
	if (!list->head || !list->head->go)
		return (0);
	primitive->name = ft_strdup(list->head->go->name);
	primitive->vao = list->head->go->vao;
	primitive->vbo = list->head->go->vbo;
	primitive->count = list->head->go->vtx_count;
	list->head->go->vao = 0;
	list->head->go->vbo = 0;
	remove_go_node(list, GO_ID_OFFSET, 1);
	return (1);
}

unsigned int		init_primitives(unsigned int nb, const char *path, \
	t_geometry *primitive)
{
	unsigned int	i;
	char			*fullpath;
	char			**file_names;
	t_go_list		list;

	if (!(file_names = ft_get_file_names(path, nb, 1)))
		return (0);
	list.head = NULL;
	list.count = 0;
	i = 0;
	while (i < nb)
	{
		fullpath = ft_strjoin(path, file_names[i]);
		parse_file(&list, NULL, fullpath, parse_wavefrontobj);
		if (!get_geometry(primitive + i, &list))
			return (0);
		i++;
		free(fullpath);
	}
	ft_free_file_names(file_names, nb);
	return (1);
}

unsigned int		init_textures(unsigned int nb, const char *path, GLuint *id)
{
	unsigned int	i;
	char			**file_names;
	char			*fullpath;

	if (!(file_names = ft_get_file_names(path, nb, 0)))
		return (0);
	i = 0;
	while (i < nb)
	{
		fullpath = ft_strjoin(path, file_names[i]);
		glActiveTexture(GL_TEXTURE0 + TEX_TEXUNIT_OFFSET + i);
		if (!(*(id + i) = create_texture(fullpath, get_tga_texture)))
		{
			free(fullpath);
			ft_free_file_names(file_names, nb);
			return (0);
		}
		free(fullpath);
		i++;
	}
	ft_free_file_names(file_names, nb);
	return (1);
}

unsigned int		init_skyboxes(unsigned int nb, const char *path, GLuint *id)
{
	unsigned int	i;
	char			**file_names;
	char			*fullpath;

	if (!(file_names = ft_get_file_names(path, nb, 0)))
		return (0);
	i = 0;
	while (i < nb)
	{
		fullpath = ft_strjoin_rf(path, ft_strjoin_rf(file_names[i], \
			ft_strjoin("/", file_names[i])));
		glActiveTexture(GL_TEXTURE0 + SKY_TEXUNIT_OFFSET + i);
		if (!(*(id + i) = create_skybox(fullpath, get_tga_texture)))
		{
			free(fullpath);
			ft_free_file_names(file_names, nb);
			return (0);
		}
		free(fullpath);
		i++;
	}
	ft_free_file_names(file_names, nb);
	return (1);
}
