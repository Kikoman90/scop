/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 12:04:54 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/10 18:58:06 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "texture.h"
#include <stdio.h>//delet this

static void         set_tex_format(t_texture *tex, GLenum format, \
    GLint internal_format)
{
    tex->format = format;
    tex->internal_format = internal_format;
}

static unsigned int init_tga_texture(t_texture *tex, t_tga_header *header)
{
    tex->width = header->width;
    tex->height = header->height;
    tex->texels = NULL;
    if (header->image_type == 3 || header->image_type == 11)
    {
        if (header->pixel_depth == 8)
            set_tex_format(tex, GL_RED, 1);
        else
            set_tex_format(tex, GL_RG, 2);
    }
    else if (header->image_type == 1 || header->image_type == 2 \
            || header->image_type == 9 || header->image_type == 10)
    {
        if (header->pixel_depth <= 24)
            set_tex_format(tex, GL_RGB, 3);
        else
            set_tex_format(tex, GL_RGBA, 4);
    }
    else
        return (log_error(INVALID_TGA));
    if (!(tex->texels = (GLubyte*)malloc(\
        sizeof(GLubyte) * tex->width * tex->height * tex->internal_format)))
        return (log_error(MALLOC_ERROR));
    return (1);
}

static void         read_tga32(t_texture *tex, GLubyte *data)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < tex->width * tex->height)
    {
        tex->texels[(i * 4)] = data[j + 2];
        tex->texels[(i * 4) + 1] = data[j + 1];
        tex->texels[(i * 4) + 2] = data[j];
        tex->texels[(i * 4) + 3] = data[j + 3];
        i++;
        j+= 4;
    }
}

static void         read_tga24(t_texture *tex, GLubyte *data)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < tex->width * tex->height)
    {
        tex->texels[(i * 3)] = data[j + 2];
        tex->texels[(i * 3) + 1] = data[j + 1];
        tex->texels[(i * 3) + 2] = data[j];
        i++;
        j+= 3;
    }
}

unsigned int        get_tga_texture(t_texture *out_tex, const char *path)
{
    t_tga_header    header;
    GLubyte         *colormap;
    GLubyte         *data;
    GLubyte         *ptr;
    size_t          data_size;

    colormap = NULL;
    if (!(data = ft_file_map(path, &data_size)))
        return (0);
    header = *((t_tga_header*)data);
    ptr = data + (sizeof(t_tga_header) + header.id_length);
    if (header.colormap_type)
    {
        colormap = ptr;
        ptr += header.clrmap_length * (header.clrmap_size >> 3);
    }
    if (!init_tga_texture(out_tex, &header))
        return (0);
    if (header.image_type == 2 && header.pixel_depth ==  24)
    {
        printf("24 BITS\n"); // delete this
        read_tga24(out_tex, ptr);
    }
    else if (header.image_type == 2 && header.pixel_depth == 32)
    {
        printf("32 BITS\n"); // delete this
        read_tga32(out_tex, ptr);
    }
    ft_file_unmap(data, data_size, NULL);
    return (1);
}

/*if (header.image_type == 1)
    read_tga(out_tex, data, colormap, header.pixel_depth, COMPRESSED?);
image_type == 1  -> read_tga_colormap() // 8/16 bit color index (GL_RGB, 3)
image_type == 9  -> read_tga_colormap_RLE() // compressed
image_type == 2  -> read_tga() // 15,16,24,32 bit color
image_type == 10 -> read_tga_RLE() // compressed
image_type == 3  -> read_tga_grayscale() // 8 (lum, 1) or 16 (lum_a, 2)
image_type == 11 -> read_tga_grayscale_RLE() // compressed*/