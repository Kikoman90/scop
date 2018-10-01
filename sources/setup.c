/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 10:38:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/26 19:15:11 by fsidler          ###   ########.fr       */
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

static void	init_default_mtl(t_material *mtl)
{
	mtl->clr_amb = vec_init_f(0.2);
	mtl->clr_dif = vec_init_f(0.6);
	mtl->clr_spc = vec_init_f(0.3);
	mtl->expnt_spc = 60.0;
	mtl->transparency = 1.0;
}

static t_camera	init_camera(t_vec3 pos, float fov, float zn, float zf)
{
	t_camera	cam;

	cam.transform.position = pos;
	cam.transform.rotation = quat_init(0, vec3_init_xyz(0, 0, -1));
	cam.transform.scale = vec3_init_f(1);
	cam.fov = fov;
	cam.znear = zn;
	cam.zfar = zf;
	return (cam);
}

t_mat4x4	compute_proj(float fov, float aspect, float zn, float zf)
{
	t_mat4x4	proj_mat;
	float right;
	float top;
	float f;

	f = 1 / tanf(fov / 2);
	top = zn * tanf(fov * M_PI / 360.0);
	right = top * aspect;
	proj_mat = mat4x4_init();
	proj_mat.m[0] = f / aspect;
	proj_mat.m[5] = f;
	proj_mat.m[10] = (zf + zn) / (zn - zf);
	proj_mat.m[11] = (2 * zf * zn) / (zn - zf);
	proj_mat.m[14] = -1;
	proj_mat.m[15] = 0;
	/*proj_mat.m[0] = (2 * zn) / (2 * right);
	proj_mat.m[5] = (2 * zn) / (2 * top);
	proj_mat.m[]

	float temp, temp2, temp3, temp4;
    temp = 2.0 * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix[0] = temp / temp2;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;
    matrix[4] = 0.0;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0;
    matrix[7] = 0.0;
    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0;
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 0.0;*/
	return (proj_mat);
}

t_mat4x4	look_at(t_vec3 pos, t_vec3 target, t_vec3 up)
{
	t_vec3		xaxis;
	t_vec3		yaxis;
	t_vec3		zaxis;
	t_mat4x4	lookat;
	
	zaxis = vec3_norm(vec3_sub(pos, target));
	xaxis = vec3_norm(vec3_cross(up, zaxis));
	yaxis = vec3_cross(zaxis, xaxis);

	lookat = mat4x4_init();
	lookat.v[0] = vec4_init_xyzw(xaxis.x, yaxis.x, zaxis.x, 0);
	lookat.v[1] = vec4_init_xyzw(xaxis.y, yaxis.y, zaxis.y, 0);
	lookat.v[2] = vec4_init_xyzw(xaxis.z, yaxis.z, zaxis.z, 0);
	lookat.v[3] = vec4_init_xyzw(-vec3_dot(xaxis, pos), -vec3_dot(yaxis, pos)\
								, -vec3_dot(zaxis, pos), 1);
	//transpose ?? apparently opengl is column major
	return (lookat);
}

t_mat4x4	compute_view(t_camera cam)
{
	t_mat4x4	view;
	t_vec3		axis;

	view = quat_to_mat4x4(cam.transform.rotation);
	//view = mat4x4_inv(view);
	axis = vec3_init_xyz(view.m[0], view.m[4], view.m[8]);
	view.m[12] = -vec3_dot(axis, cam.transform.position);
	axis = vec3_init_xyz(view.m[1], view.m[5], view.m[9]);
	view.m[13] = -vec3_dot(axis, cam.transform.position);
	axis = vec3_init_xyz(view.m[2], view.m[6], view.m[10]);
	view.m[14] = -vec3_dot(axis, cam.transform.position);// or no sign
	return (view);
	//mat.data[12] = -dot(vec3_init_xyz(view.m[0], view.m[]), position);
	//mat.data[13] = -dot(u, position);
	//mat.data[14] = dot(f, position);
}

char		*read_shader(const char *path, int fd, size_t data_size)
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
	data_size = (size_t)s.st_size;
	if ((data = (char*)mmap(NULL, data_size, \
		PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		log_error_free(ft_strjoin("(mmap) ", strerror(errno)));
		close(fd);
		return (NULL);
	}
	close(fd);
	return (data);
}

/*
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
		*/

t_shader	init_shader(const char *path)
{
	//create_shader(path + '.extension', SHADER_TYPE, ...);
	t_shader		shader;
	const GLchar	*src[1];
	GLint			lengths[1];
	GLint			success;
	GLchar			info_log[1024];

	shader.name = ft_strrchr(path, '/') + 1;
	if ((src[0] = read_shader(path + ".vert", 0, 0)) == NULL)
		return (NULL);
	lengths[0] = ft_strlen((char*)src[0]);
	if ((shader.vert_s = glCreateShader(GL_VERTEX_SHADER)) == 0)
	{
		log_error_free(ft_strjoin_auto((char**){SHADER_CREATE_ERROR, shader.name, "-", shader_type, ")"}, (int*){0, 0, 0, 0, 0}));
		// WTF;
		// need to clean stuff up; functions are jut scattered around files
		// get a render then clean up
		log_error_free(ft_strjoin_rf(SHADER_CREATE_ERROR, ft_strjoin_rf(shader.name, ft_strjoin_rf("-", ))))
	}
	glShaderSource(shader.vert_s, 1, src, lengths);

}

t_env		*init_scop(t_env *env, int argc, char **argv)
{
	t_mtl_node	*def_mtl;

	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (log_error_null(MALLOC_ERROR));
	if (!init_sdl_gl(env))
		return (clean_scop(env, CLEAN_SDL));
	
	env->camera = init_camera(vec3_init_xyz(0, 0, 3), 60, 0.1, 25);
	//look_at(mat4x4_t(env->camera->transform), vec_init_f(0), vec_init_xyz(0, 1, 0));

	env->projection_mat = compute_proj(env->camear.fov, \
		WIN_W / WIN_H, env->camera.znear, env->camera.zfar);

	// not here
	env->view_mat = compute_view(env->camera); // do this before each frame (if cam.transform.hasMoved())

	env->def_shader = init_shader("resources/shaders/default");

	env->go_count = 0;
	env->go_list = NULL;
	env->mtl_count = 0;
	def_mtl = create_mtl_node(ft_strdup("default_mtl"));
	init_default_mtl(def_mtl->mtl);
	env->mtl_list = add_mtl_node(env, def_mtl);
	while (argc-- > 1)
		parse_file(env, argv[argc], parse_wavefrontobj);
	env->loop = 1;
	return (env);
}
