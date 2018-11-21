/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_inter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 20:44:01 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/21 20:51:50 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static unsigned int get_mouse_ray(t_env *env, t_ray *ray, int x, int y)
{
    t_vec2          viewport_size;
    t_vec2          coord;
    t_vec3          axes[3];
    unsigned int    mouse_down;

    mouse_down = 0;
    if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
        mouse_down = 1;
    coord.x = (2.0f * x - env->win_env.win_w) / env->win_env.win_w;
    coord.y = (env->win_env.win_h - 2.0f * y) / env->win_env.win_h;
    get_matrix_axes(&axes, \
        mat4x4_transpose(quat_to_mat4x4(env->camera.transform.rotation)));
    viewport_size.y = tanf(env->camera.fov * M_PI / 360) * env->camera.znear;
    viewport_size.x = viewport_size.y * env->win_env.win_w / env->win_env.win_h;
    ray->origin = env->camera.transform.position;
    axes[2] = vec3_scale(axes[2], -1);
    axes[2] = vec3_add(ray->origin, vec3_scale(axes[2], env->camera.znear));
    axes[2] = vec3_add(axes[2], vec3_scale(axes[1], viewport_size.y * coord.y));
    axes[2] = vec3_add(axes[2], vec3_scale(axes[0], viewport_size.x * coord.x));
    ray->dir = vec3_norm(vec3_sub(ray->origin, axes[2]));
    return (mouse_down);
}

static void         rotate_handles_inter(t_selection *sel, t_ray ray, \
    double t, int mouse_down)
{
    t_obj_param obj_p;
    int         type;
    //int         i;

    type = -1;
    obj_p.pos = sel->transform.position;
    obj_p.radius = sel->transform.scale.x;
    if (sphere_inter(ray, obj_p, &t))
        type = 0;
    /*i = 1;
    while (i < 4)
    {
        obj_p.set...
        if (cylinder_inter(ray, obj_p, &t))
            type = i;
        i++;
    }*/
    selection_def_colors(&sel->colors);
    if (mouse_down && type != -1)
    {
        vec4_v3(&sel->colors[type], vec3_norm((t_vec3)SCOP_MDOWN));
        sel->active = 1;
    }
    else if (type != -1)
        vec4_v3(&sel->colors[type], vec3_norm((t_vec3)SCOP_MOVER));        
    sel->type = type;
}

unsigned int        handles_inter(t_env *env)
{
    t_ray   ray;
    double  t;
    int     mouse_down;

    t = (double)env->camera.zfar;
    mouse_down = get_mouse_ray(env, &ray, 0, 0);
    /* try applying inverse matrix to the ray instead of the primitive object
    t_mat4x4 ma = mat4x4_trs(sel->offset[0], sel->quat[0], sel->scale[2]);
    ma = mat4x4_mult(mat4x4_trs(sel->transform.position, \
        sel->transform.rotation, sel->transform.scale), ma);*/
    if (env->selection.mode & SCOP_TRANSLATE)
        ;//translate_handles_inter(&env->selection, &ray, t, mouse_down);
    else if (env->selection.mode & SCOP_ROTATE)
        rotate_handles_inter(&env->selection, ray, t, mouse_down);
    else if (env->selection.mode & SCOP_SCALE)
        ;//scale_handles_inter(&env->selection, &ray, t, mouse_down);
    return (0);
}
