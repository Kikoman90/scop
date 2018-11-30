/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_manip.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 17:06:07 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/30 18:56:33 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

// SCOP_TRANSLATE, SCOP_ROTATE, SCOP_SCALE sel_transform();
void        sel_transform(t_selection *sel, t_vec3 delta)
{
    t_tr_node   *tmp;

    tmp = sel->list.head;
    while (tmp)
    {
        if (sel->mode & SCOP_TRANSLATE)
            tmp->transform->position = \
                vec3_add(tmp->transform->position, delta);
        else if (tmp->id != 1 && (sel->mode & SCOP_ROTATE)) //
            ; //
        else if (tmp->id != 1)
            tmp->transform->scale = vec3_add(tmp->transform->scale, delta);
        tmp = tmp->next;
    }
}

void        selection_rotate(t_selection *sel, t_quaternion rot)
{
    t_tr_node   *tmp;

    tmp = sel->list.head;
    while (tmp)
    {
        if (tmp->id != 1)
            tmp->transform->rotation = quat_mult(tmp->transform->rotation, rot);
        tmp = tmp->next;
    }
}

static void t_manip(t_selection *sel, t_ray ray)
{
    double  t;
    float   delta;
    t_vec3  pos;
    t_vec3  delta_v;

    t = T_MAX;
    if (!plane_inter(ray, sel->last_pos, sel->view_axis, &t))
        return ;
    pos = vec3_add(ray.origin, vec3_scale(ray.dir, t));
    delta = vec3_dot(sel->proj_axis, vec3_sub(pos, sel->last_pos));
    delta_v = vec3_scale(sel->motion_axis, delta);
    sel_transform(sel, delta_v);
    sel->last_pos = pos;
}

static void r_manip(t_selection *sel, t_ray ray)
{
    double          t;
    float           delta;
    t_vec3          pos;
    t_quaternion    delta_q;

    t = T_MAX;
    if (!plane_inter(ray, sel->last_pos, sel->view_axis, &t))
        return ;
    pos = vec3_add(ray.origin, vec3_scale(ray.dir, t));    
    delta = (360.0f * vec3_length(vec3_sub(pos, sel->last_pos))) / \
        (2.0f * M_PI * sel->transform.scale.x * sel->scale[2].x);
    if (sel->type == 0)
        sel->motion_axis = vec3_norm(\
            vec3_cross(sel->view_axis, vec3_sub(pos, sel->last_pos)));
    else
        delta *= vec3_dot(sel->proj_axis, vec3_norm(vec3_sub(pos, sel->last_pos)));
    delta_q = quat_tv(-delta, sel->motion_axis);
    selection_rotate(sel, delta_q);
    sel->last_pos = pos;
}

// FIND A WAY. JUST FIND A WAY. AND THEN CLEAN EVERYTHING UP AND LETS GET ON WITH IT
// LETS GET ON WITH IT !
// https://youtu.be/K5M4hiat8WI?t=2274
// https://youtu.be/K5M4hiat8WI?t=2274
// https://youtu.be/K5M4hiat8WI?t=2274
static void s_manip(t_selection *sel, t_ray ray)
{
    double  t;
    float   delta;
    t_vec3  pos;
    t_vec3  delta_v;

    t = T_MAX;
    if (!plane_inter(ray, sel->last_pos, sel->view_axis, &t))
        return ;
    pos = vec3_add(ray.origin, vec3_scale(ray.dir, t));
    if (sel->type == 0)
    {
        delta_v = vec3_sub(pos, sel->last_pos);
        delta = vec3_length(delta_v);
        //delta = vec3_sub(delta_v, vec3_scale(sel->view_axis, vec3_dot(delta_v, sel->view_axis)));
        //delta_v = vec3_f(delta);
        sel->offset[1].x += 8 * delta;
        sel->offset[2].y += 8 * delta;
        sel->offset[3].z += 8 * delta;
        sel->scale[0] = vec3_add(sel->scale[0], delta_v);
    }
    else
    {
        delta = vec3_dot(sel->proj_axis, vec3_sub(pos, sel->last_pos));
        delta_v = vec3_scale(sel->motion_axis, delta);
        if (sel->type == 1)
        {
            sel->scale[0].x += delta;
            sel->offset[sel->type].x += 8 * delta;
        }
        else if (sel->type == 2)
        {
            sel->scale[0].y += delta;
            sel->offset[sel->type].y += 8 * delta;
        }
        else
        {
            sel->scale[0].z += delta;
            sel->offset[sel->type].z += 8 * delta;   
        }
    }
    sel_transform(sel, delta_v);
    sel->last_pos = pos;
}

void        handles_manip(t_env *env, int x, int y)
{
    t_ray   ray;

    get_mouse_ray(env, &ray, x, y);
    if (env->selection.mode & SCOP_TRANSLATE)
        t_manip(&env->selection, ray);
    else if (env->selection.mode & SCOP_ROTATE)
        r_manip(&env->selection, ray);
    else if (env->selection.mode & SCOP_SCALE)
        s_manip(&env->selection, ray);
    set_selection_mode(&env->selection, env->selection.mode, 0);
}
