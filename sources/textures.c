/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 18:31:13 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/16 19:58:25 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>//
#include "texture.h"

short           	clrmap_index(t_tga_header *header, GLubyte *clrmap, \
    GLubyte *data, short *pixel)
{
    short   idx;

    if (header->pixel_depth >= 15 && header->pixel_depth <= 16)
        idx = data[0] + (data[1] << 8);
    else if (header->pixel_depth == 8)
        idx = data[0];
    else
        idx = 0;
    *pixel = ((clrmap[idx * 2] + (clrmap[(idx * 2) + 1] << 8)) << 1) + 1;
    return (idx);
}

unsigned int    	texture_format(t_texture *tex, GLenum format, \
  GLint internal_format, unsigned int bpp)
{
    if (bpp == 2)
        tex->pixel_type = GL_UNSIGNED_SHORT_5_5_5_1;
    else
        tex->pixel_type = GL_UNSIGNED_BYTE;
    tex->format = format;
    tex->internal_format = internal_format;
    return (bpp);
}

GLuint				create_texture(const char *path, \
	unsigned int (get_texture)(t_texture *, const char *))
{
	t_texture	tex;

	if (!get_texture(&tex, path))
		return (0);
	glGenTextures(1, &tex.id);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, tex.internal_format, tex.width, \
		tex.height, 0, tex.format, tex.pixel_type, &tex.texels[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (tex.format == GL_RED)
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, \
			(GLint[]){GL_RED, GL_RED, GL_RED, GL_ALPHA});
	else if (tex.format == GL_RG)
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, \
			(GLint[]){GL_RED, GL_RED, GL_RED, GL_GREEN});
	free(tex.texels);
	return (tex.id);
}

static unsigned int	skybox_side(const char *path, const char ext[4], \
	unsigned int idx, unsigned int (get_texture)(t_texture *, const char *))
{
	t_texture	tex;
	char		*fullpath;

	fullpath = ft_strjoin_rf(path, ft_strjoin(ext, ".tga"));
	if (!get_texture(&tex, fullpath))
	{
		free(fullpath);
		return (0);
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + idx, 0, \
		tex.internal_format, tex.width, tex.height, 0, tex.format, \
		tex.pixel_type, tex.texels);
	free(fullpath);
	free(tex.texels);
	return (1);
}

static GLuint		create_skybox(const char *path, \
	unsigned int (get_texture)(t_texture *, const char *))
{
	unsigned int	i;
	GLuint			id;
	const char		ext[6][4] = { "_rt", "_lf", "_up", "_dn", "_ft", "_bk" };

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	i = 0;
    while (i < 6)
    {
		if (!skybox_side(path, ext[i], i, get_texture))
		{
			glDeleteTextures(1, &id);
			return (0);
		}
		i++;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return (id);
}

unsigned int	init_textures(unsigned int nb, const char *path, GLuint *id)
{
	unsigned int	i;
	char			**file_names;
	char			*fullpath;

	if (!(file_names = ft_get_file_names(path, nb)))
		return (0);
	i = 0;
	while (i < nb)
	{
		fullpath = ft_strjoin(path, file_names[i]);
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

// get primitives vao (cube, sphere, cone, circle (line_loop), line...)
/*void			init_sky_vao(GLuint *vao_id)
{
	t_go_list		*list;
	t_gameobject	*go;

	list->head = NULL;
	list->count = 0;
	parse_file(&list, NULL, "resources/obj/cube,obj", parse_wavefrontobj);
	go = get_gameobject(list->head, GO_ID_OFFSET);

}
*/

unsigned int	init_skyboxes(unsigned int nb, const char *path, GLuint *id)
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