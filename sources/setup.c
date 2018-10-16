/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 10:38:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/16 19:06:52 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

// update.c file
t_mat4x4	compute_proj(float fov, float aspect, float zn, float zf)
{
	t_mat4x4	proj_mat;
	float f;

	fov *= M_PI / 180.0f;
	f = 1 / tanf(fov / 2);
	proj_mat = mat4x4();
	proj_mat.m[0] = f / aspect;
	proj_mat.m[5] = f;
	proj_mat.m[10] = -(zf + zn) / (zf - zn);
	proj_mat.m[11] = -1;
	proj_mat.m[14] = (-2 * zn * zf) / (zf - zn);
	proj_mat.m[15] = 0;
	return (proj_mat);
}

/*
** glGetIntegerv(GL_MAX_SAMPLES, &max_samples); // multisampling
** printf("max samples : %d\n", max_samples); // macos = 8;
*/
static int	check_framebuffer_status(const char *fbo_type)
{
	GLenum	status;
	char	*err_msg;

	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) == GL_FRAMEBUFFER_COMPLETE)
		return (1);
	err_msg = ft_strjoin(fbo_type, FRAMEBUFFER_INCOMPLETE_ERROR);
	switch (status)
	{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			log_error_free(ft_strjoin_lf(err_msg, " (incomplete attachment)"));
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			log_error_free(ft_strjoin_lf(err_msg, " (missing attachment)"));
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			log_error_free(ft_strjoin_lf(err_msg, " (samples do not match)"));
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			log_error_free(ft_strjoin_lf(err_msg, " (internal format unsupported)"));
			break;
		default:
			log_error_free(err_msg);
	}
	return (0);
}

static int	init_framebuffers(GLuint *ms_fbo, GLuint *pick_fbo)
{
	GLuint	rbo[4];

	glGenFramebuffers(1, ms_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, *ms_fbo);
	glGenRenderbuffers(4, &rbo[0]);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo[0]);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB8, WIN_W, WIN_H);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo[0]);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo[1]);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, WIN_W, WIN_H);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo[1]);
	if (check_framebuffer_status("multisample ") == 0)
		return (0);
	glGenFramebuffers(1, pick_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, *pick_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo[2]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, WIN_W, WIN_H);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo[2]);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo[3]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIN_W, WIN_H);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo[3]);
	return (check_framebuffer_status("color pick "));
}

static void	*init_sdl_gl(t_env *env)
{
	env->ms_fbo = 0;
	env->pick_fbo = 0;
	env->def_shader.prog = 0;
	env->pick_shader.prog = 0;
	env->std_shader.prog = 0;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		return (log_error_null(SDL_INIT_ERROR));
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, \
		SDL_GL_CONTEXT_PROFILE_CORE);
	if (!(env->window = SDL_CreateWindow("SCOP", SDL_WINDOWPOS_CENTERED, \
		SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_RESIZABLE \
		| SDL_WINDOW_OPENGL)))
		return (log_error_null(WIN_CREATE_ERROR));
	if (!(env->gl_context = SDL_GL_CreateContext(env->window)))
		return (log_error_null(SDL_GetError()));
	init_framebuffers(&env->ms_fbo, &env->pick_fbo);
	glClearColor(0.19, 0.27, 0.41, 1);
	glViewport(0, 0, WIN_W, WIN_H);
	return ((void*)1);
}

static t_camera	init_camera(t_vec3 pos, float fov, float zn, float zf)
{
	t_camera	cam;

	cam.transform.position = pos;
	cam.transform.rotation = quat();
	cam.transform.scale = vec3_f(1);
	cam.fov = fov;
	cam.znear = zn;
	cam.zfar = zf;
	return (cam);
}

t_env			*init_scop(t_env *env, int argc, char **argv)
{
	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (log_error_null(MALLOC_ERROR));
	if (!init_sdl_gl(env))
		return (clean_scop(env, CLEAN_SDL_GL));
	env->camera = init_camera(vec3_xyz(0, 0, 3), 80, 0.1, 30);
	env->proj_mat = compute_proj(env->camera.fov, \
		(float)WIN_W / WIN_H, env->camera.znear, env->camera.zfar);
	if (!init_program(&env->def_shader, "resources/shaders/default") ||\
		!init_program(&env->pick_shader, "resources/shaders/pick") ||\
		!init_program(&env->std_shader, "resources/shaders/standard"))
		return (clean_scop(env, CLEAN_SDL_GL));
	env->mtl_list = NULL;
	env->go_list = NULL;
	env->selection = NULL;
	env->mtl_count = 0;
	env->go_count = 0;
	env->selection_count = 0;
	while (argc-- > 1)
		parse_file(env, argv[argc], parse_wavefrontobj);
	env->loop = 1;
	return (env);
}
