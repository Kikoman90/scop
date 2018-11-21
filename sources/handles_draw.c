/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 20:06:14 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/21 13:39:43 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	draw_translate_handles(t_selection *sel, t_shader *shader, \
	t_geometry line_geo, t_geometry cone_geo)
{
	glUniform4fv(shader->u_loc[4], 3, &sel->colors[1].x);
	glBindVertexArray(line_geo.vao);
	sel->rotations[1] = mat4x4_trs(sel->offset[1], sel->quat[3], sel->scale[2]);
	sel->rotations[2] = mat4x4_trs(sel->offset[2], sel->quat[0], sel->scale[2]);
	sel->rotations[3] = mat4x4_trs(sel->offset[3], sel->quat[1], sel->scale[2]);
	glUniformMatrix4fv(shader->u_loc[3], 3, GL_FALSE, &sel->rotations[1].m[0]);
	glDrawArraysInstanced(GL_LINES, 0, line_geo.count, 3);
	glBindVertexArray(cone_geo.vao);
	sel->rotations[1] = mat4x4_trs(sel->offset[4], sel->quat[3], sel->scale[1]);
	sel->rotations[2] = mat4x4_trs(sel->offset[5], sel->quat[0], sel->scale[1]);
	sel->rotations[3] = mat4x4_trs(sel->offset[6], sel->quat[1], sel->scale[1]);
	glUniformMatrix4fv(shader->u_loc[3], 3, GL_FALSE, &sel->rotations[1].m[0]);
	glDrawArraysInstanced(GL_TRIANGLES, 0, cone_geo.count, 3);
}

static void	draw_rotate_handles(t_selection *sel, t_shader *shader, \
	t_geometry sphere_geo, t_geometry circle_geo)
{
	glUniform4fv(shader->u_loc[4], 1, &sel->colors[0].x);
	glBindVertexArray(sphere_geo.vao);
	sel->rotations[0] = mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[2]);
	glUniformMatrix4fv(shader->u_loc[3], 1, GL_FALSE, &sel->rotations[0].m[0]);
	glDrawArraysInstanced(GL_TRIANGLES, 0, sphere_geo.count, 1);
	glBindVertexArray(circle_geo.vao);
	glUniform4fv(shader->u_loc[4], 3, &sel->colors[1].x);
	sel->rotations[1] = mat4x4_trs(sel->offset[0], sel->quat[2], sel->scale[3]);
	sel->rotations[2] = mat4x4_trs(sel->offset[0], sel->quat[1], sel->scale[3]);
	sel->rotations[3] = mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[3]);
	glUniformMatrix4fv(shader->u_loc[3], 3, GL_FALSE, &sel->rotations[1].m[0]);
	glDrawArraysInstanced(GL_LINES, 0, circle_geo.count, 3);
}

static void	draw_scale_handles(t_selection *sel, t_shader *shader, \
	t_geometry line_geo, t_geometry cube_geo)
{
	glUniform4fv(shader->u_loc[4], 3, &sel->colors[1].x);
	glBindVertexArray(line_geo.vao);
	sel->rotations[1] = mat4x4_trs(sel->offset[1], sel->quat[3], sel->scale[2]);
	sel->rotations[2] = mat4x4_trs(sel->offset[2], sel->quat[0], sel->scale[2]);
	sel->rotations[3] = mat4x4_trs(sel->offset[3], sel->quat[1], sel->scale[2]);
	glUniformMatrix4fv(shader->u_loc[3], 3, GL_FALSE, &sel->rotations[1].m[0]);
	glDrawArraysInstanced(GL_LINES, 0, line_geo.count, 3);
	glUniform4fv(shader->u_loc[4], 4, &sel->colors[0].x);
	glBindVertexArray(cube_geo.vao);
	sel->rotations[0] = mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[1]);
	sel->rotations[1] = mat4x4_trs(sel->offset[4], sel->quat[3], sel->scale[0]);
	sel->rotations[2] = mat4x4_trs(sel->offset[5], sel->quat[0], sel->scale[0]);
	sel->rotations[3] = mat4x4_trs(sel->offset[6], sel->quat[1], sel->scale[0]);
	glUniformMatrix4fv(shader->u_loc[3], 4, GL_FALSE, &sel->rotations[0].m[0]);
	glDrawArraysInstanced(GL_TRIANGLES, 0, cube_geo.count, 4);
}

void		draw_handles(t_env *env, t_shader *shader, float scale)
{
	glUseProgram(shader->prog);
	if (env->selection.mode & SCOP_TRANSLATE)
	{
		env->selection.transform.scale = vec3_f(0.125f * scale);
		set_uniforms(env, shader, NULL);
		draw_translate_handles(&env->selection, shader, \
			env->primitives[SCOP_LINE], env->primitives[SCOP_CONE]);
	}
	else if (env->selection.mode & SCOP_ROTATE)
	{
		env->selection.transform.scale = vec3_f(scale);
		set_uniforms(env, shader, NULL);
		draw_rotate_handles(&env->selection, shader, \
			env->primitives[SCOP_SPHERE], env->primitives[SCOP_CIRCLE]);
	}
	else if (env->selection.mode & SCOP_SCALE)
	{
		env->selection.transform.scale = vec3_f(0.125f * scale);
		set_uniforms(env, shader, NULL);
		draw_scale_handles(&env->selection, shader, \
			env->primitives[SCOP_LINE], env->primitives[SCOP_CUBE]);
	}
}
