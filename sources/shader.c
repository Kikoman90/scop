/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 14:28:46 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/15 19:15:22 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static GLuint	shader_error(const char *shader_name, char *log, \
	GLenum shader_type)
{
	char	*file_name;
	char	*info_log;
	char	*error_msg;

	if (shader_type == GL_VERTEX_SHADER)
		file_name = ft_strjoin(shader_name, ".vert");
	else if (shader_type == GL_FRAGMENT_SHADER)
		file_name = ft_strjoin(shader_name, ".frag");
	else
		file_name = ft_strdup(shader_name);
	info_log = ft_strjoin("\n", log);
	error_msg = ft_strjoin_bf(ft_strjoin(SHADER_INIT_ERROR, " ("), \
		ft_strjoin_bf(ft_strjoin_lf(file_name, ")"), info_log));
	return ((GLuint)log_error_free(error_msg));
}

static GLuint	shader_source(const char *name, char *data, GLenum shader_type)
{
	GLuint			sh;
	GLint			success;
	GLchar			info_log[1024];
	const GLchar	*src[1];

	if (!data)
		return (0);
	if (!(sh = glCreateShader(shader_type)))
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

static GLuint	get_shaders(t_shader *prg, const char *path, \
	unsigned int u_loc_count)
{
	char			*fullpath;
	char			*fdata;
	size_t			fsize;

	fullpath = ft_strjoin(path, ".vert");
	fdata = ft_file_map(fullpath, &fsize);
	if ((prg->vtx_s = shader_source(prg->name, fdata, GL_VERTEX_SHADER)) == 0)
	{
		ft_file_unmap(fdata, fsize, fullpath);
		return (0);
	}
	ft_file_unmap(fdata, fsize, fullpath);
	fullpath = ft_strjoin(path, ".frag");
	fdata = ft_file_map(fullpath, &fsize);
	if ((prg->frg_s = shader_source(prg->name, fdata, GL_FRAGMENT_SHADER)) == 0)
	{
		ft_file_unmap(fdata, fsize, fullpath);
		glDeleteShader(prg->vtx_s);
		return (0);
	}
	ft_file_unmap(fdata, fsize, fullpath);
	while (u_loc_count)
		prg->u_loc[--u_loc_count] = -1;
	return (1);
}

static GLuint	create_program(t_shader *program, const char *path, \
	char *name)
{
	GLint	success;
	GLchar	info_log[1024];

	program->name = ft_strdup(name);
	if (get_shaders(program, path, 13) == 0)
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
	get_uniforms(program);
	return (1);
}

unsigned int	init_shaders(unsigned int nb, const char *path, \
	t_shader *shader)
{
	unsigned int	i;
	char			**file_names;
	char			*fullpath;

	if (!(file_names = ft_get_file_names(path, nb)))
		return (0);
	i = 0;
	while (i < nb)
	{
		fullpath = ft_strjoin_rf(path, ft_strjoin_rf(file_names[i], \
			ft_strjoin("/", file_names[i])));
		if (!create_program(shader + i, fullpath, file_names[i]))
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