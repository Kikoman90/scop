/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 10:38:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/09 14:35:26 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void		parse_file(t_env *env, const char *path, \
						void (*ft_parsing)(t_env*, t_parser*, char*))
{
	t_parser	*parser;
	int			fd;

	if (!(parser = (t_parser*)malloc(sizeof(t_parser))))
		log_error(MALLOC_ERROR);
	if (init_parser(parser, path, &fd) != -1)
	{
		if ((parser->data = (char *)mmap(NULL, parser->fsize, \
			PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			log_error_free(ft_strjoin("(mmap) ", strerror(errno)));
		else
		{
			parser->data[parser->fsize] = '\0';
			ft_parsing(env, parser, NULL);
			if (munmap(parser->data, parser->fsize) == -1)
				log_error_free(ft_strjoin("(munmap) ", strerror(errno)));
			parser->data = NULL;
		}
		close(fd);
		free(parser->fpath);
	}
	free(parser);
}

static void	*init_sdl_gl(t_env *env)
{
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
	glClearColor(0.19, 0.27, 0.41, 1);
	glViewport(0, 0, WIN_W, WIN_H);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	return ((void*)1);
}

static t_mtl_node	*init_default_mtl(void)
{
	t_mtl_node	*def_mtl;
		
	def_mtl = create_mtl_node(ft_strdup("default_mtl"));
	def_mtl->mtl->clr_amb = vec3_f(0.2);
	def_mtl->mtl->clr_dif = vec3_f(0.6);
	def_mtl->mtl->clr_spc = vec3_f(0.3);
	def_mtl->mtl->expnt_spc = 60.0;
	def_mtl->mtl->transparency = 1.0;
	return (def_mtl);
}

static t_camera	init_camera(t_vec3 pos, float fov, float zn, float zf)
{
	t_camera	cam;

	cam.transform.position = pos;
	cam.transform.rotation = quat_tv(0, vec3_xyz(0, 0, 1));
	cam.transform.scale = vec3_f(1);
	cam.fov = fov;
	cam.znear = zn;
	cam.zfar = zf;
	return (cam);
}

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
	proj_mat.m[11] = (-2 * zn * zf) / (zf - zn);
	proj_mat.m[14] = -1;
	proj_mat.m[15] = 0;
	return (proj_mat);
}

char		*read_shader_file(const char *path, size_t *data_size, int fd)
{
	struct stat	s;
	char		*data;

	if ((fd = open(path, O_RDWR)) == -1)
	{
		log_error_free(ft_strjoin("(open) ", strerror(errno)));
		return (NULL);
	}
	if(fstat(fd, &s) == -1)
	{
		close(fd);
		log_error_free(ft_strjoin("(fstat) ", strerror(errno)));
		return (NULL);
	}
	*data_size = (size_t)s.st_size;
	if ((data = (char*)mmap(NULL, *data_size, \
		PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		log_error_free(ft_strjoin("(mmap) ", strerror(errno)));
		close(fd);
		return (NULL);
	}
	close(fd);
	return (data);
}

GLuint		create_shader(const char *name, char *data, GLenum shader_type)
{
	GLuint	sh;
	GLint	success;
	GLchar	info_log[1024];
	const GLchar	*src[1];
	// PUT SIZE HERE AND ALLOC LOG TO THE RIGHT SIZE ??

	if (!data || (sh = glCreateShader(shader_type)) == 0)
		return (0);
	src[0] = (const GLchar*)data;
	glShaderSource(sh, 1, src, NULL);
	glCompileShader(sh);
	glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(sh, 1024, NULL, &info_log[0]);
		glDeleteShader(sh);
		return (shader_error(name, info_log, shader_type));
	}
	return (sh);
}

void		free_data_and_path(char *data, char *fpath, size_t fsize)
{
	free(fpath);
	fpath = NULL;
	if (data)
	{
		if (munmap(data, fsize) == -1)
			log_error_free(ft_strjoin("(munmap) ", strerror(errno)));
		data = NULL;
	}
}

GLuint		init_shaders(t_shader *prg, const char *path)
{
	//const GLchar	*src[1];
	char			*fullpath;
	char			*fdata;
	size_t			fsize;

	fullpath = ft_strjoin(path, ".vert");
	fdata = read_shader_file(fullpath, &fsize, 0);
	//src[0] = (const GLchar*)fdata;
	if ((prg->vtx_s = create_shader(prg->name, fdata, GL_VERTEX_SHADER)) == 0)
	{
		free_data_and_path(fdata, fullpath, fsize);
		return (0);
	}
	free_data_and_path(fdata, fullpath, fsize);
	fullpath = ft_strjoin(path, ".frag");
	fdata = read_shader_file(fullpath, &fsize, 0);
	//src[0] = (const GLchar*)fdata;
	if ((prg->frg_s = create_shader(prg->name, fdata, GL_FRAGMENT_SHADER)) == 0)
	{
		free_data_and_path(fdata, fullpath, fsize);
		glDeleteShader(prg->vtx_s);
		return (0);
	}
	free_data_and_path(fdata, fullpath, fsize);
	return (1);
}

GLuint		init_program(t_shader *program, const char *path)
{
	GLint	success;
	GLchar	info_log[1024];

	program->name = ft_strrchr(path, '/') + 1;
	if (init_shaders(program, path) == 0)
		return (0);
	program->prog = glCreateProgram();
	glAttachShader(program->prog, program->vtx_s);
	glAttachShader(program->prog, program->frg_s);
	glLinkProgram(program->prog);
	glGetProgramiv(program->prog, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetProgramInfoLog(program->prog, 1024, NULL, &info_log[0]);
		glDeleteProgram(program->prog);
		glDeleteShader(program->vtx_s);
		glDeleteShader(program->frg_s);
		return (shader_error(program->name, info_log, 0));
	}
	glDetachShader(program->prog, program->vtx_s);
	glDetachShader(program->prog, program->frg_s);
	return (1);
}

t_env		*init_scop(t_env *env, int argc, char **argv)
{
	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (log_error_null(MALLOC_ERROR));
	if (!init_sdl_gl(env))
		return (clean_scop(env, CLEAN_SDL));
	env->camera = init_camera(vec3_xyz(0, 0, 3), 80, 0.1, 3);//0.1 && 25 (pos = {0, 0, -15})
	env->proj_mat = compute_proj(env->camera.fov, \
		(float)WIN_W / WIN_H, env->camera.znear, env->camera.zfar);
	//(float)(WIN_W / WIN_H)
	if (init_program(&(env->def_shader), "resources/shaders/default") == 0)
		return (clean_scop(env, CLEAN_SDL));
	env->go_count = 0;
	env->go_list = NULL;
	env->mtl_count = 0;
	env->mtl_list = NULL;
	env->mtl_list = add_mtl_node(env, init_default_mtl());
	while (argc-- > 1)
		parse_file(env, argv[argc], parse_wavefrontobj);
	env->loop = 1;
	return (env);
}
