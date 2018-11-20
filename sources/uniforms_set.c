/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniforms_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 13:47:36 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/20 16:35:47 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	set_default_shader_uniforms(t_env *env, t_shader *shader, int id)
{
    glUniformMatrix4fv(shader->u_loc[0], 1, GL_FALSE, \
        &env->matrices.model[id - GO_ID_OFFSET].m[0]);
    glUniformMatrix4fv(shader->u_loc[1], 1, GL_FALSE, &env->matrices.v.m[0]);
    glUniformMatrix4fv(shader->u_loc[2], 1, GL_FALSE, &env->matrices.p.m[0]);
    glUniform3fv(shader->u_loc[3], 1, &env->light.transform.position.x);
    glUniform3fv(shader->u_loc[4], 1, &env->light.color[env->input.cur_sky].x);
    glUniform1f(shader->u_loc[5], env->light.intensity);
    glUniform1f(shader->u_loc[6], env->light.range);
	glUniform1i(shader->u_loc[7], 1 + env->input.cur_tex);
    glUniform3fv(shader->u_loc[8], 1, &env->camera.transform.position.x);
    glUniform1f(shader->u_loc[9], env->input.fade);
    glUniform1f(shader->u_loc[10], env->input.face_rgb);
}

static void	set_standard_shader_uniforms(t_env *env, t_shader *shader, int id, \
    t_material *mtl)
{
	glUniformMatrix4fv(shader->u_loc[0], 1, GL_FALSE, \
        &env->matrices.model[id - GO_ID_OFFSET].m[0]);
    glUniformMatrix4fv(shader->u_loc[1], 1, GL_FALSE, &env->matrices.v.m[0]);
    glUniformMatrix4fv(shader->u_loc[2], 1, GL_FALSE, &env->matrices.p.m[0]);
    glUniform3fv(shader->u_loc[3], 1, &env->light.transform.position.x);
    glUniform3fv(shader->u_loc[4], 1, &env->light.color[env->input.cur_sky].x);
    glUniform1f(shader->u_loc[5], env->light.intensity);
    glUniform1f(shader->u_loc[6], env->light.range);
	glUniform3fv(shader->u_loc[7], 1, &mtl->clr_amb.x);
	glUniform3fv(shader->u_loc[8], 1, &mtl->clr_dif.x);
	glUniform3fv(shader->u_loc[9], 1, &mtl->clr_spc.x);
	glUniform1f(shader->u_loc[10], mtl->expnt_spc);
	glUniform1f(shader->u_loc[11], mtl->transparency);
    glUniform3fv(shader->u_loc[12], 1, &env->camera.transform.position.x);
}

static void set_pick_shader_uniforms(t_env *env, t_shader *shader, \
    t_go_node *node)
{
    glUniformMatrix4fv(shader->u_loc[1], 1, GL_FALSE, &env->matrices.v.m[0]);
    glUniformMatrix4fv(shader->u_loc[2], 1, GL_FALSE, &env->matrices.p.m[0]);
    if (node)
    {
        glUniformMatrix4fv(shader->u_loc[0], 1, GL_FALSE, \
            &env->matrices.model[node->id - GO_ID_OFFSET].m[0]);
        glUniform1i(shader->u_loc[3], 0);
        glUniform3fv(shader->u_loc[4], 1, &node->go->pick_clr.x);
    }
    else
    {
        glUniformMatrix4fv(shader->u_loc[0], 1, GL_FALSE, \
            &(go_trs(env->light.transform)).m[0]);
        glUniform1i(shader->u_loc[3], 1);        
        glUniform3fv(shader->u_loc[4], 1, &env->light.pick_clr.x);
    }
}

static void set_billboard_shader_uniforms(t_env *env, t_shader *shader)
{
    glUniformMatrix4fv(shader->u_loc[0], 1, GL_FALSE, \
        &(go_trs(env->light.transform)).m[0]);
    glUniformMatrix4fv(shader->u_loc[1], 1, GL_FALSE, &env->matrices.v.m[0]);
    glUniformMatrix4fv(shader->u_loc[2], 1, GL_FALSE, &env->matrices.p.m[0]);
    glUniform1i(shader->u_loc[3], 0);
}

void		set_uniforms(t_env *env, t_shader *shdr, t_go_node *node)
{
    if (ft_strcmp(shdr->name, "default") == 0)
        set_default_shader_uniforms(env, shdr, node->id);
    else if (ft_strcmp(shdr->name, "standard") == 0)
        set_standard_shader_uniforms(env, shdr, node->id, \
            get_mtl(env->materials.head, node->go->mtl_id));
    else if (ft_strcmp(shdr->name, "pick") == 0)
        set_pick_shader_uniforms(env, shdr, node);
    else if (ft_strcmp(shdr->name, "billboard") == 0)
        set_billboard_shader_uniforms(env, shdr);
    else if (ft_strcmp(shdr->name, "primitive") == 0)
    {
        glUniformMatrix4fv(shdr->u_loc[0], 1, GL_FALSE, \
            &(go_trs(env->selection.transform)).m[0]);
        glUniformMatrix4fv(shdr->u_loc[1], 1, GL_FALSE, &env->matrices.v.m[0]);
        glUniformMatrix4fv(shdr->u_loc[2], 1, GL_FALSE, &env->matrices.p.m[0]);
        //glUniformMatrix4fv(shdr->u_loc[3], 4, GL_FALSE, &env->selection.rot.m[0]);
        //glUniform4fv(shdr->u_loc[4, 4, &env->selection.colors.x);
        glUniform1i(shdr->u_loc[5], env->selection.localspace);
    }
    else if (ft_strcmp(shdr->name, "skybox") == 0)
    {
        glUniformMatrix4fv(shdr->u_loc[0], 1, GL_FALSE, &env->matrices.v.m[0]);
        glUniformMatrix4fv(shdr->u_loc[1], 1, GL_FALSE, &env->matrices.p.m[0]);
        glUniform1i(shdr->u_loc[2], 8 + env->input.cur_sky);
    }
}
