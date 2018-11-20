/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniforms_get.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 13:47:20 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/20 14:29:18 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void get_default_shader_uniforms(t_shader *shader)
{
    shader->u_loc[0] = glGetUniformLocation(shader->prog, "m");
	shader->u_loc[1] = glGetUniformLocation(shader->prog, "v");
	shader->u_loc[2] = glGetUniformLocation(shader->prog, "p");
    shader->u_loc[3] = glGetUniformLocation(shader->prog, "light.position");
    shader->u_loc[4] = glGetUniformLocation(shader->prog, "light.color");
    shader->u_loc[5] = glGetUniformLocation(shader->prog, "light.intensity");
    shader->u_loc[6] = glGetUniformLocation(shader->prog, "light.range");
    shader->u_loc[7] = glGetUniformLocation(shader->prog, "tex");
    shader->u_loc[8] = glGetUniformLocation(shader->prog, "view_pos");
    shader->u_loc[9] = glGetUniformLocation(shader->prog, "fade");
    shader->u_loc[10] = glGetUniformLocation(shader->prog, "rgb");
}

static void get_standard_shader_uniforms(t_shader *shader)
{
    shader->u_loc[0] = glGetUniformLocation(shader->prog, "m");
	shader->u_loc[1] = glGetUniformLocation(shader->prog, "v");
	shader->u_loc[2] = glGetUniformLocation(shader->prog, "p");
    shader->u_loc[3] = glGetUniformLocation(shader->prog, "light.position");
    shader->u_loc[4] = glGetUniformLocation(shader->prog, "light.color");
    shader->u_loc[5] = glGetUniformLocation(shader->prog, "light.intensity");
    shader->u_loc[6] = glGetUniformLocation(shader->prog, "light.range");
    shader->u_loc[7] = glGetUniformLocation(shader->prog, "mat.clr_amb");
    shader->u_loc[8] = glGetUniformLocation(shader->prog, "mat.clr_dif");
    shader->u_loc[9] = glGetUniformLocation(shader->prog, "mat.clr_spc");
    shader->u_loc[10] = glGetUniformLocation(shader->prog, "mat.expnt_spc");
    shader->u_loc[11] = glGetUniformLocation(shader->prog, "mat.transp");
    shader->u_loc[12] = glGetUniformLocation(shader->prog, "view_pos");
}

static void get_pick_shader_uniforms(t_shader *shader)
{
    shader->u_loc[3] = glGetUniformLocation(shader->prog, "billboard");
    shader->u_loc[4] = glGetUniformLocation(shader->prog, "pick_clr");
}

static void get_primtive_shader_uniforms(t_shader *shader)
{
    shader->u_loc[3] = glGetUniformLocation(shader->prog, "rot");
    shader->u_loc[4] = glGetUniformLocation(shader->prog, "clr");
    shader->u_loc[5] = glGetUniformLocation(shader->prog, "localspace");
}

void		get_uniforms(t_shader *shader)
{
    if (ft_strcmp(shader->name, "default") == 0)
        get_default_shader_uniforms(shader);
    else if (ft_strcmp(shader->name, "standard") == 0)
        get_standard_shader_uniforms(shader);
    else if (ft_strcmp(shader->name, "billboard") == 0 ||\
            ft_strcmp(shader->name, "pick") == 0 ||\
            ft_strcmp(shader->name, "primitive") == 0)
    {
        shader->u_loc[0] = glGetUniformLocation(shader->prog, "m");
        shader->u_loc[1] = glGetUniformLocation(shader->prog, "v");
        shader->u_loc[2] = glGetUniformLocation(shader->prog, "p");
        if (ft_strcmp(shader->name, "billboard") == 0)
            shader->u_loc[3] = glGetUniformLocation(shader->prog, "tex");
        else if (ft_strcmp(shader->name, "pick") == 0)
            get_pick_shader_uniforms(shader);
        else
            get_primtive_shader_uniforms(shader);
    }
    else if (ft_strcmp(shader->name, "skybox") == 0)
    {
        shader->u_loc[0] = glGetUniformLocation(shader->prog, "v");
        shader->u_loc[1] = glGetUniformLocation(shader->prog, "p");
        shader->u_loc[2] = glGetUniformLocation(shader->prog, "skybox");
    }
}
