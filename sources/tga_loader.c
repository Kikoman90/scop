/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 12:04:54 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/16 22:26:53 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "texture.h"

static void         read_tga_mapped(t_texture *tex, t_tga_header *header, \
    GLubyte *data, unsigned int bpp)
{
    int             i;
    short           idx;
    short           pixel;
    GLubyte         *clrmap;

    i = 0;
    clrmap = data + sizeof(t_tga_header) + header->id_length + header->clrmap_first;
    data = clrmap + (header->clrmap_length * (header->clrmap_size >> 3));
    while (i < tex->width * tex->height)
    {
        if ((idx = clrmap_index(header, clrmap, data + i, &pixel)) && bpp == 2)
        {
            tex->texels[i * 2] = (GLubyte)pixel;
            tex->texels[(i * 2) + 1] = (GLubyte)(pixel >> 8);
        }
        else
        {
            tex->texels[i * bpp] = clrmap[idx * bpp];
            tex->texels[(i * bpp) + 1] = clrmap[(idx * bpp) + 1];
            tex->texels[(i * bpp) + 2] = clrmap[(idx * bpp) + 2];
            if (tex->format == GL_RGBA)
                tex->texels[(i * bpp) + 3] = clrmap[(idx * bpp) + 3];
        }
        i++;
    }
}

static void         read_tga_grayscale(t_texture *tex, t_tga_header *header, \
    GLubyte *data, unsigned int bpp)
{
    unsigned int    i;

    i = 0;
    data += sizeof(t_tga_header) + header->id_length;
    while (i < tex->width * tex->height * bpp)
    {
        tex->texels[i] = data[i];
        if (tex->format == GL_RG)
            tex->texels[i + 1] = data[i + 1];
        i += bpp;
    }
}

static void         read_tga_truecolor(t_texture *tex, t_tga_header *header, \
    GLubyte *data, unsigned int bpp)
{
    unsigned int    i;
    short           pixel;

    i = 0;
    data += sizeof(t_tga_header) + header->id_length;
    while (i < tex->width * tex->height * bpp)
    {
        if (bpp == 2)
        {
            pixel = ((data[i] + (data[i + 1] << 8)) << 1) + 0x0001;
            tex->texels[i] = (GLubyte)pixel;
            tex->texels[i + 1] = (GLubyte)(pixel >> 8);
        }
        else
        {
            tex->texels[i] = data[i + 2];
            tex->texels[i + 1] = data[i + 1];
            tex->texels[i + 2] = data[i];
            if (tex->format == GL_RGBA)
                tex->texels[i + 3] = data[i + 3];
        }
        i += bpp;
    }
}

static unsigned int set_tga_format(t_texture *tex, t_tga_header *header)
{
    GLubyte pixel_depth;
    GLubyte image_type;

    pixel_depth = (header->image_type == 1 || header->image_type == 9) ? \
        header->clrmap_size : header->pixel_depth;
    if ((image_type = header->image_type) == 1 || image_type == 2)
	{
		if (pixel_depth == 15)
            return (texture_format(tex, GL_RGB, GL_RGB5, 2));
		else if (pixel_depth == 16)
		    return (texture_format(tex, GL_RGBA, GL_RGB5_A1, 2));
		else if (pixel_depth == 24)
		    return (texture_format(tex, GL_RGB, GL_RGB8, 3));
		else if (pixel_depth == 32)
            return (texture_format(tex, GL_RGBA, GL_RGBA8, 4));
	}
    else if (image_type == 3)
    {
        if (pixel_depth == 8)
            return (texture_format(tex, GL_RED, GL_R8, 1));
        else if (pixel_depth == 16)
            return (texture_format(tex, GL_RG, GL_RG8, 2));
    }
	return (log_error(INVALID_TGA));
}

unsigned int        get_tga_texture(t_texture *out_tex, const char *path)
{
	t_tga_header    header;
	GLubyte         *data;
	size_t          data_size;
    unsigned int    bpp;

	if (!(data = ft_file_map(path, &data_size)))
		return (0);
	header = *((t_tga_header*)data);
	out_tex->width = header.width;
	out_tex->height = header.height;
	out_tex->texels = NULL;
	if (!(bpp = set_tga_format(out_tex, &header)))
        return (0);
	if (!(out_tex->texels = (GLubyte*)malloc(\
        sizeof(GLubyte) * out_tex->width * out_tex->height * bpp)))
		return (log_error(MALLOC_ERROR));
	if (header.image_type == 1)
	    read_tga_mapped(out_tex, &header, data, bpp);
	else if (header.image_type == 2)
	    read_tga_truecolor(out_tex, &header, data, bpp);
	else if (header.image_type == 3)
	    read_tga_grayscale(out_tex, &header, data, bpp);
	ft_file_unmap(data, data_size, NULL);
	return (1);
}