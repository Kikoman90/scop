/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 20:06:14 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/20 20:17:24 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	set_selection_rotations(t_mat4x4 (*rot)[4], float offset, float scale, int orthogonal)
{
	if (orthogonal)
	{
		(*rot)[0] = mat4x4_trs(vec3_xyz(0, offset, offset), \
			quat_tv(90, (t_vec3)VEC3_UP), vec3_f(scale));
		(*rot)[1] = mat4x4_trs(vec3_xyz(offset, 0, offset), \
			quat_tv(-90, (t_vec3)VEC3_RIGHT), vec3_f(scale));
		(*rot)[2] = mat4x4_trs(vec3_xyz(offset, offset, 0), \
			quat(), vec3_f(scale));
	}
	else
	{
		(*rot)[0] = mat4x4_trs(vec3_xyz(offset, 0, 0), \
			quat_tv(90, (t_vec3)VEC3_FRONT), vec3_f(scale));
		(*rot)[1] = mat4x4_trs(vec3_xyz(0, offset, 0), quat(), vec3_f(scale));
		(*rot)[2] = mat4x4_trs(vec3_xyz(0, 0, offset), \
			quat_tv(-90, (t_vec3)VEC3_RIGHT), vec3_f(scale));
	}
	(*rot)[3] = mat4x4_trs((t_vec3)VEC3_ZERO, quat(), vec3_f(scale + 0.1f));
}

// float[4] mouseover/mouseclick (state)
static void	set_selection_colors(t_vec4 (*clr)[4], float alpha)
{
	(*clr)[0] = vec4_v3w(vec3_norm(vec3_xyz(0.9f, 0.1f, 0.1f)), alpha);
	(*clr)[1] = vec4_v3w(vec3_norm(vec3_xyz(0.1f, 0.9f, 0.1f)), alpha);
	(*clr)[2] = vec4_v3w(vec3_norm(vec3_xyz(0.1f, 0.1f, 0.9f)), alpha);
	(*clr)[3] = vec4_v3w(vec3_norm(vec3_f(0.3f)), alpha - 0.1f);
}

void        draw_translate_handles(t_env *env, t_shader *shd)
{
	glBindVertexArray(env->primitives[SCOP_CONE].vao);
	set_selection_rotations(&env->selection.rot, 8, 0.9f, 0);
	set_selection_colors(&env->selection.clr, 1);
	glUniformMatrix4fv(shd->u_loc[3], 3, GL_FALSE, &env->selection.rot[0].m[0]);
	glUniform4fv(shd->u_loc[4], 3, &env->selection.clr[0].x);
	glDrawArraysInstanced(GL_TRIANGLES, 0, env->primitives[SCOP_CONE].count, 3);
	glBindVertexArray(env->primitives[SCOP_LINE].vao);
	set_selection_rotations(&env->selection.rot, 4, 1, 0);
	glUniformMatrix4fv(shd->u_loc[3], 3, GL_FALSE, &env->selection.rot[0].m[0]);
	glDrawArraysInstanced(GL_LINES, 0, env->primitives[SCOP_LINE].count, 3);
	glDisable(GL_CULL_FACE);
	glBindVertexArray(env->primitives[SCOP_PLANE].vao);
	set_selection_rotations(&env->selection.rot, 1, 1, 1); //dot,dot,dot
	set_selection_colors(&env->selection.clr, 0.7f);
	glUniformMatrix4fv(shd->u_loc[3], 3, GL_FALSE, &env->selection.rot[0].m[0]);
	glUniform4fv(shd->u_loc[4], 3, &env->selection.clr[0].x);
	glDrawArraysInstanced(\
		GL_TRIANGLES, 0, env->primitives[SCOP_PLANE].count, 3);
	glEnable(GL_CULL_FACE);
}

void        draw_rotate_handles(t_env *env, t_shader *shd)
{
	glBindVertexArray(env->primitives[SCOP_SPHERE].vao);
	env->selection.rot[0] = mat4x4_trs(vec3_f(0), quat(), vec3_f(1));
	// if selected -> switch color to dim yellow
	env->selection.clr[0] = vec4_v3w(vec3_f(0.3f), 0.3f);
	glUniformMatrix4fv(shd->u_loc[3], 1, GL_FALSE, &env->selection.rot[0].m[0]);
	glUniform4fv(shd->u_loc[4], 1, &env->selection.clr[0].x);
	glDrawArraysInstanced(\
		GL_TRIANGLES, 0, env->primitives[SCOP_SPHERE].count, 1);
	glBindVertexArray(env->primitives[SCOP_CIRCLE].vao);
	set_selection_rotations(&env->selection.rot, 0, 1.005f, 1);
	set_selection_colors(&env->selection.clr, 1);
	glUniformMatrix4fv(shd->u_loc[3], 3, GL_FALSE, &env->selection.rot[0].m[0]);
	glUniform4fv(shd->u_loc[4], 3, &env->selection.clr[0].x);
	glDrawArraysInstanced(GL_LINES, 0, env->primitives[SCOP_CIRCLE].count, 3);
}

void        draw_scale_handles(t_env *env, t_shader *shd)
{
	glBindVertexArray(env->primitives[SCOP_CUBE].vao);
	set_selection_rotations(&env->selection.rot, 8, 0.5f, 0);
	set_selection_colors(&env->selection.clr, 0.8f);
	glUniformMatrix4fv(shd->u_loc[3], 4, GL_FALSE, &env->selection.rot[0].m[0]);
	glUniform4fv(shd->u_loc[4], 4, &env->selection.clr[0].x);
	glDrawArraysInstanced(GL_TRIANGLES, 0, env->primitives[SCOP_CUBE].count, 4);
	glBindVertexArray(env->primitives[SCOP_LINE].vao);
	set_selection_rotations(&env->selection.rot, 4, 1, 0);
	glUniformMatrix4fv(shd->u_loc[3], 3, GL_FALSE, &env->selection.rot[0].m[0]);
	glDrawArraysInstanced(GL_LINES, 0, env->primitives[SCOP_LINE].count, 3);
}
