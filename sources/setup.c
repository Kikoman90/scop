/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 10:38:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/15 19:20:54 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void			init_default(t_env *env)
{
	unsigned int	i;

	env->buffers.ms_fbo = 0;
	env->buffers.pick_fbo = 0;
	i = 4;
	while (i--)
		env->buffers.rbo[i] = 0;
	while (i < 6)
	{
		env->shaders[i].prog = 0;
		env->primitives[i].vao = 0;
		env->primitives[i].vbo = 0;
		env->textures[i++] = 0;
	}
	i = 0;
	while (i < 2)
		env->skyboxes[i++] = 0;
	env->matrices.model = NULL;
	env->materials.head = NULL;
	env->materials.count = 0;
	env->gameobjects.head = NULL;
	env->gameobjects.count = 0;
	env->selection.head = NULL;
	env->selection.count = 0;
}

static unsigned int	init_sdl_gl(t_win *win)
{
	win->win_w = WIN_W;
	win->win_h = WIN_H;
	win->window = NULL;
	win->gl_context = NULL;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		return (log_error(SDL_INIT_ERROR));
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, \
		SDL_GL_CONTEXT_PROFILE_CORE);
	if (!(win->window = SDL_CreateWindow("SCOP_42", SDL_WINDOWPOS_CENTERED, \
		SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_INPUT_GRABBED |\
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL)))
		return (log_error(WIN_CREATE_ERROR));
	if (!(win->gl_context = SDL_GL_CreateContext(win->window)))
		return (log_error(SDL_GetError()));
	SDL_WarpMouseInWindow(win->window, WIN_W / 2, WIN_H / 2);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, WIN_W, WIN_H);
	return (1);
}

static t_camera		init_camera(t_vec3 pos, float fov, float zn, float zf)
{
	t_camera	cam;

	cam.transform.position = pos;
	cam.transform.rotation = quat();
	cam.transform.scale = (t_vec3)VEC3_ONE;
	cam.fov = fov;
	cam.znear = zn;
	cam.zfar = zf;
	return (cam);
}

static unsigned int	init_light(t_light *light, t_vec3 color, float intensity, \
	float range)
{
	if (!(light->texture_id = \
		create_texture("resources/gizmos/light.tga", get_tga_texture)))
		return (0);
	light->transform = init_transform_trs(vec3_xyz(-3.0f, 1.2f, 0.0f), \
		quat(), (t_vec3)VEC3_ONE);
	light->color = color;
	light->intensity = intensity;
	light->range = range;
	light->id = 1;
	light->pick_clr = generate_pick_clr(light->id);
	return (1);
}

void				init_input(t_inputstate *input)
{
	input->cur_tex = 0;
	input->cur_sky = 0;
	input->pan_speed = 5.0f;
	input->zoom_speed = 1.0f;
	input->orbit_speed = 15.0f;
	input->fade = 1.0f;
	input->face_rgb = 0;
	input->auto_rotate = 1;
	input->localspace = 1;
	input->selection_mode = SCOP_TRANSLATE;
}

unsigned int		init_primitives(unsigned int nb, const char *path, \
	t_vertex_data *primitive)
{
	t_go_list		list;
	unsigned int	i;
	char			**file_names;
	char			*fullpath;

	if (!(file_names = ft_get_file_names(path, nb)))
		return (0);
	list.head = NULL;
	list.count = 0;
	i = 0;
	while (i < nb)
	{
		fullpath = ft_strjoin(path, file_names[i]);
		parse_file(&list, NULL, fullpath, parse_wavefrontobj);
		(primitive + i)->vao = list.head->go->vertex_data.vao;
		(primitive + i)->vbo = list.head->go->vertex_data.vbo;
		(primitive + i++)->count = list.head->go->vertex_data.count;
		free(list.head->go->vtx_attrib);
		free(list.head->go->name);
		free(list.head->go);
		remove_go_node(&list, GO_ID_OFFSET, 0);
		free(fullpath);
	}
	ft_free_file_names(file_names, nb);
	return (1);
}

unsigned int		init_scop(t_env *env, int argc, char **argv)
{
	init_default(env);
	if (!init_sdl_gl(&env->win_env))
		return (0);
	glGenFramebuffers(1, &env->buffers.ms_fbo);
	glGenFramebuffers(1, &env->buffers.pick_fbo);
	glGenRenderbuffers(4, &env->buffers.rbo[0]);
	if (!generate_framebuffers(&env->buffers, WIN_W, WIN_H) ||\
		!init_shaders(6, "resources/shaders/", &env->shaders[0]) ||\
		!init_primitives(1, "resources/primitives/", &env->primitives[0]) ||\
		!init_textures(3, "resources/textures/", &env->textures[0]) ||\
		!init_skyboxes(1, "resources/skyboxes/", &env->skyboxes[0]) ||\
		!init_light(&env->light, vec3_xyz(0.96f, 0.6f, 0.4f), 1.5f, 10))
		return (0);
	env->camera = init_camera(vec3_xyz(0, 0, 3), 80, 0.001f, 50);
	env->matrices.update_mat[0] = 1;
	env->matrices.update_mat[1] = 1;
	env->matrices.update_mat[2] = 1;
	while (argc-- > 1)
		parse_file(&env->gameobjects, &env->materials, argv[argc], \
			parse_wavefrontobj);
	init_input(&env->input);
	env->loop = 1;
	return (1);
}