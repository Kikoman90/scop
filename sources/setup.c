/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 10:38:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 19:18:53 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void			init_default(t_env *env)
{
	unsigned int	i;

	env->buffers.ms_fbo = 0;
	env->buffers.pick_fbo = 0;
	ft_bzero(env->buffers.rbo, 4);
	i = 0;
	while (i < 6)
	{
		env->shaders[i].prog = 0;
		env->primitives[i].vao = 0;
		env->primitives[i++].vbo = 0;
	}
	ft_bzero(env->textures, 7);
	ft_bzero(env->skyboxes, 4);
	env->light.texture_id = 0;
	env->matrices.model = NULL;
	env->materials.head = NULL;
	env->materials.count = 0;
	env->gameobjects.head = NULL;
	env->gameobjects.count = 0;
	env->selection.list.head = NULL;
	env->selection.list.count = 0;
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
		SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_RESIZABLE |\
		SDL_WINDOW_OPENGL)))
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

static void			init_input(t_inputstate *input)
{
	input->cur_tex = 0;
	input->cur_sky = 2;
	input->pan_speed = 1.0f;
	input->rot_speed = 15.0f;
	input->zoom_speed = 1.0f;
	input->fade = 1.0f;
	input->face_rgb = 0;
	input->auto_rotate = 1;
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
		!init_primitives(6, "resources/primitives/", &env->primitives[0]) ||\
		!init_textures(7, "resources/textures/", &env->textures[0]) ||\
		!init_skyboxes(4, "resources/skyboxes/", &env->skyboxes[0]) ||\
		!init_light(&env->light, vec3_xyz(-3, 1.2f, 0), 2.5f, 20))
		return (0);
	env->camera = init_camera(vec3_xyz(0, 0, 3), FOV, ZNEAR, ZFAR);
	env->matrices.update_mat[0] = 1;
	env->matrices.update_mat[1] = 1;
	env->matrices.update_mat[2] = 1;
	while (argc-- > 1)
		parse_file(&env->gameobjects, &env->materials, argv[argc], \
			parse_wavefrontobj);
	init_input(&env->input);
	init_selection(&env->selection);
	env->loop = 1;
	return (1);
}
