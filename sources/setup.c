/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 10:38:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/10 18:52:14 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void			init_default(t_env *env)
{
	env->win_env.window = NULL;
	env->win_env.gl_context = NULL;
	env->win_env.win_w = WIN_W;
	env->win_env.win_h = WIN_H;
	env->buffers.rbo[0] = 0;
	env->buffers.rbo[1] = 0;
	env->buffers.rbo[2] = 0;
	env->buffers.rbo[3] = 0;
	env->buffers.ms_fbo = 0;
	env->buffers.pick_fbo = 0;
	env->shaders[0].prog = 0;
	env->shaders[1].prog = 0;
	env->shaders[2].prog = 0;
	env->matrices.model = NULL;
	env->matrices.update_mat[0] = 1;
	env->matrices.update_mat[1] = 1;
	env->matrices.update_mat[2] = 1;
	env->materials.head = NULL;
	env->materials.count = 0;
	env->gameobjects.head = NULL;
	env->gameobjects.count = 0;
	env->selection.head = 0;
	env->selection.count = 0;
}

static unsigned int	init_sdl_gl(t_win *win)
{
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
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, WIN_W, WIN_H);
	return (1);
}

/*
** SDL_SetRelativeMouseMode(SDL_TRUE);
** SDL_SetWindowGrab(win->window, SDL_TRUE);
** glEnable(GL_TEXTURE_2D);
*/

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

static t_light		init_light(t_vec3 color, float intensity, float range)
{
	t_light	light;

	light.transform = init_transform_trs(vec3_xyz(0.0f, 1.2f, 0.0f), \
		quat(), (t_vec3)VEC3_ONE);
	light.color = color;
	light.intensity = intensity;
	light.range = range;
	return (light);
}

unsigned int		init_scop(t_env *env, int argc, char **argv)
{
	init_default(env);
	if (!init_sdl_gl(&env->win_env))
		return (0);
	glGenFramebuffers(1, &env->buffers.ms_fbo);
	glGenFramebuffers(1, &env->buffers.pick_fbo);
	glGenRenderbuffers(4, &env->buffers.rbo[0]);
	if (!generate_framebuffers(&env->buffers, WIN_W, WIN_H))
		return (0);
		/*
		*/
	if (!get_tga_texture(&env->tex, "resources/textures/yousavedme.tga"))
		return (0);
	glGenTextures(1, &env->tex.id);
	glBindTexture(GL_TEXTURE_2D, env->tex.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, env->tex.width, \
		env->tex.height, 0, env->tex.format, GL_UNSIGNED_BYTE, env->tex.texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	free(env->tex.texels);
	env->fade = 0.0f;
		/*
		*/
	if (!init_program(&env->shaders[0], "resources/shaders/default", 0) || \
		!init_program(&env->shaders[1], "resources/shaders/pick", 1) || \
		!init_program(&env->shaders[2], "resources/shaders/standard", 2))
		return (0);
	env->camera = init_camera(vec3_xyz(0, 0, 3.0f), 80.0f, 0.001f, 50.0f);
	env->light = init_light((t_vec3)VEC3_ONE, 1.5f, 20.0f);
	while (argc-- > 1)
		parse_file(&env->gameobjects, &env->materials, argv[argc], \
			parse_wavefrontobj);
	env->input.pan_speed = 5.0f;
	env->input.zoom_speed = 1.0f;
	env->input.orbit_speed = 15.0f;
	env->loop = 1;
	return (1);
}
