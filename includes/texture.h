/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 11:36:52 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/10 18:14:02 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_H
# define TEXTURE_H

# include "../libs/libft/includes/libft.h"

# include <OpenGL/gl3.h>

# define MALLOC_ERROR "memory allocation failure"
# define INVALID_TGA "invalid or unsupported TGA format"

typedef struct  s_texture
{
    GLsizei     width;
    GLsizei     height;
    GLenum      format;
    GLint       internal_format;
    GLuint      id;
    GLubyte     *texels;
}               t_texture;

#pragma pack(push, 1)
typedef struct  s_tga_header
{
    GLubyte     id_length;
    GLubyte     colormap_type;
    GLubyte     image_type;
    short       clrmap_first;
    short       clrmap_length;
    GLubyte     clrmap_size;
    short       x_origin;
    short       y_origin;
    short       width;
    short       height;
    GLubyte     pixel_depth;
    GLubyte     image_descriptor;
}               t_tga_header;
#pragma pack(pop)

unsigned int    get_tga_texture(t_texture *out_tex, const char *path);

#endif