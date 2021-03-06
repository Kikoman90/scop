/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 11:36:52 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 19:26:11 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_H
# define TEXTURE_H

# include "../libs/libft/includes/libft.h"

# include <OpenGL/gl3.h>

# define INVALID_TGA "invalid or unsupported TGA format"

# define TEX_TEXUNIT_OFFSET 1
# define SKY_TEXUNIT_OFFSET 8

typedef struct	s_texture
{
	GLsizei		width;
	GLsizei		height;
	GLenum		pixel_type;
	GLenum		format;
	GLint		internal_format;
	GLuint		id;
	GLubyte		*texels;
}				t_texture;

# pragma pack(push, 1)

typedef struct	s_tga_header
{
	GLubyte		id_length;
	GLubyte		colormap_type;
	GLubyte		image_type;
	short		clrmap_first;
	short		clrmap_length;
	GLubyte		clrmap_size;
	short		x_origin;
	short		y_origin;
	short		width;
	short		height;
	GLubyte		pixel_depth;
	GLubyte		image_descriptor;
}				t_tga_header;

# pragma pack(pop)

/*
** tga_loader.c => 5 functions
*/
unsigned int	get_tga_texture(t_texture *out_tex, const char *path);

/*
** textures.c	=> 5 functions
*/
short			clrmap_index(t_tga_header *header, GLubyte *clrmap, \
					GLubyte *data, short *pixel);
unsigned int	texture_format(t_texture *tex, GLenum format, \
					GLint internal_format, unsigned int bpp);
GLuint			create_texture(const char *path, \
					unsigned int (get_texture)(t_texture *, const char *));
GLuint			create_skybox(const char *path, \
					unsigned int (get_texture)(t_texture *, const char *));

#endif
