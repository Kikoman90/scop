/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 14:28:46 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/31 10:38:43 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void		free_data_and_path(char *data, char *fpath, size_t fsize)
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

static char		*read_shader_file(const char *path, size_t *data_size, int fd)
{
	struct stat	s;
	char		*data;

	if ((fd = open(path, O_RDWR)) == -1)
	{
		log_error_free(ft_strjoin("(open) ", strerror(errno)));
		return (NULL);
	}
	if (fstat(fd, &s) == -1)
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

static GLuint	create_shader(const char *name, char *data, GLenum shader_type)
{
	GLuint			sh;
	GLint			success;
	GLchar			info_log[1024];
	const GLchar	*src[1];

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

static GLuint	init_shaders(t_shader *prg, const char *path, \
	unsigned int u_loc_count)
{
	char			*fullpath;
	char			*fdata;
	size_t			fsize;

	fullpath = ft_strjoin(path, ".vert");
	fdata = read_shader_file(fullpath, &fsize, 0);
	if ((prg->vtx_s = create_shader(prg->name, fdata, GL_VERTEX_SHADER)) == 0)
	{
		free_data_and_path(fdata, fullpath, fsize);
		return (0);
	}
	free_data_and_path(fdata, fullpath, fsize);
	fullpath = ft_strjoin(path, ".frag");
	fdata = read_shader_file(fullpath, &fsize, 0);
	if ((prg->frg_s = create_shader(prg->name, fdata, GL_FRAGMENT_SHADER)) == 0)
	{
		free_data_and_path(fdata, fullpath, fsize);
		glDeleteShader(prg->vtx_s);
		return (0);
	}
	free_data_and_path(fdata, fullpath, fsize);
	while (u_loc_count)
		prg->u_loc[--u_loc_count] = -1;
	return (1);
}

GLuint			init_program(t_shader *program, const char *path, t_uniforms uf)
{
	GLint	success;
	GLchar	info_log[1024];

	program->name = ft_strrchr(path, '/') + 1;
	if (init_shaders(program, path, 12) == 0)
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
	glDeleteShader(program->vtx_s);
	glDeleteShader(program->frg_s);
	get_uniforms(program, uf);
	return (1);
}
