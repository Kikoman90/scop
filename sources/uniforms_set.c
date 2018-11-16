/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniforms_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 13:47:36 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/16 21:47:03 by fsidler          ###   ########.fr       */
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, env->textures[env->input.cur_tex]);
	glUniform1i(shader->u_loc[7], 0);
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
    t_mat4x4    light_mat;

    if (node)
    {
        glUniformMatrix4fv(shader->u_loc[0], 1, GL_FALSE, \
            &env->matrices.model[node->id - GO_ID_OFFSET].m[0]);
        glUniform3fv(shader->u_loc[3], 1, &node->go->pick_clr.x);
    }
    else
    {
        light_mat = go_trs(env->light.transform);
        glUniformMatrix4fv(shader->u_loc[0], 1, GL_FALSE, &light_mat.m[0]);
        glUniform3fv(shader->u_loc[3], 1, &env->light.pick_clr.x);
    }
    glUniformMatrix4fv(shader->u_loc[1], 1, GL_FALSE, &env->matrices.v.m[0]);
    glUniformMatrix4fv(shader->u_loc[2], 1, GL_FALSE, &env->matrices.p.m[0]);
}

static void set_gizmos_shader_uniforms(t_env *env, t_shader *shader)
{
    t_mat4x4    light_matrix;

    light_matrix = go_trs(env->light.transform);
    glUniformMatrix4fv(shader->u_loc[0], 1, GL_FALSE, &light_matrix.m[0]);
    glUniformMatrix4fv(shader->u_loc[1], 1, GL_FALSE, &env->matrices.v.m[0]);
    glUniformMatrix4fv(shader->u_loc[2], 1, GL_FALSE, &env->matrices.p.m[0]);
    glUniform1i(shader->u_loc[3], 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, env->light.texture_id);
    glUniform1i(shader->u_loc[4], 1);
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
    else if (ft_strcmp(shdr->name, "gizmos") == 0)
        set_gizmos_shader_uniforms(env, shdr);
    else if (ft_strcmp(shdr->name, "primitive") == 0)
    {
        //glUniformMatrix4fv(shdr->u_loc[0], 1, GL_FALSE, &env->selection);
        glUniformMatrix4fv(shdr->u_loc[1], 1, GL_FALSE, &env->matrices.v.m[0]);
        glUniformMatrix4fv(shdr->u_loc[2], 1, GL_FALSE, &env->matrices.p.m[0]);
        glUniform1i(shdr->u_loc[3], env->input.localspace);
    }
    else if (ft_strcmp(shdr->name, "skybox") == 0)
    {
        glUniformMatrix4fv(shdr->u_loc[0], 1, GL_FALSE, &env->matrices.v.m[0]);
        glUniformMatrix4fv(shdr->u_loc[1], 1, GL_FALSE, &env->matrices.p.m[0]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, env->skyboxes[env->input.cur_sky]);
        glUniform1i(shdr->u_loc[2], 2);
    }
}