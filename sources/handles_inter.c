/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles_inter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 20:44:01 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/27 20:35:27 by fsidler          ###   ########.fr       */
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
    coord.x = (2.0f * --x - env->win_env.win_w) / env->win_env.win_w;
    coord.y = (env->win_env.win_h - 2.0f * --y) / env->win_env.win_h;
    get_matrix_axes(&axes, \
        mat4x4_transpose(quat_to_mat4x4(env->camera.transform.rotation)));
    viewport_size.y = tanf(env->camera.fov * M_PI / 360) * env->camera.znear;
    viewport_size.x = viewport_size.y * env->win_env.win_w / env->win_env.win_h;
    ray->origin = env->camera.transform.position;
	axes[2] = vec3_scale(axes[2], -1);
    axes[2] = vec3_add(ray->origin, vec3_scale(axes[2], env->camera.znear));
    axes[2] = vec3_add(axes[2], vec3_scale(axes[1], viewport_size.y * coord.y));
    axes[2] = vec3_add(axes[2], vec3_scale(axes[0], viewport_size.x * coord.x));
    ray->dir = vec3_norm(vec3_sub(axes[2], ray->origin));
    return (mouse_down);
}

static void         translate_handles_inter(t_selection *sel, t_ray ray, \
    double *t)
{
    t_obj_param     obj_p;
    unsigned int    i;

    i = 1;
    while (i < 4)
    {
        obj_p.dir = vec3_norm(vec3_mat4x4_prod(mat4x4_mult(\
            go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_DOWN, 0));
        obj_p.pos = vec3_mat4x4_prod(mat4x4_mult(\
            go_trs(sel->transform), sel->rot[i]), vec3_xyz(0, 4, 0), 1);
        obj_p.height = 4.0 * sel->transform.scale.x * sel->scale[1].x;
        obj_p.radius = 2.0 * sel->transform.scale.x * sel->scale[1].x;
        if (cone_inter(ray, obj_p, atan(0.5), t))
            sel->type = i;
        obj_p.dir = vec3_norm(vec3_mat4x4_prod(mat4x4_mult(\
            go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_UP, 0));
        obj_p.pos = vec3_mat4x4_prod(mat4x4_mult(\
            go_trs(sel->transform), sel->rot[i + 3]), (t_vec3)VEC3_ZERO, 1);
        obj_p.height = 8.0 * sel->transform.scale.x * sel->scale[0].x;
        obj_p.radius = LINE_RADIUS * sel->transform.scale.x;
        if (cylinder_inter(ray, obj_p, t))
            sel->type = i;
        i++;
    }
}

static void         rotate_handles_inter(t_selection *sel, t_ray ray, double *t)
{
    t_obj_param obj_p;
    int         i;

    i = 1;
    obj_p.pos = sel->transform.position;
    obj_p.radius = 1.0 * sel->transform.scale.x * sel->scale[3].x;
    obj_p.height = 0.125 * LINE_RADIUS * sel->transform.scale.x;
    //printf("line radius = %f\n", obj_p.height);
    //*t = vec3_length(vec3_add(ray.origin, sel->transform.position));
    while (i < 4)
    {
        /*obj_p.pos = vec3_mat4x4_prod(mat4x4_mult(\
            go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_ZERO, 1);*/
        obj_p.dir = vec3_norm(vec3_mat4x4_prod(mat4x4_mult(\
            go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_FRONT, 0));
        if (torus_inter(ray, obj_p, t))
            sel->type = i;
        i++;
    }
    printf("after torus -> t = %f\n", *t);
    obj_p.radius = 1.0 * sel->transform.scale.x * sel->scale[2].x;
    if (sphere_inter(ray, obj_p, t))
        sel->type = 0;
    printf("after sphere -> t = %f\n", *t);    
}

static void         scale_handles_inter(t_selection *sel, t_ray ray, double *t)
{
    t_obj_param     obj_p;
    t_vec3          up_vec;
    unsigned int    i;

    i = 0;
    while (i < 4)
    {
        obj_p.dir = vec3_norm(vec3_mat4x4_prod(mat4x4_mult(\
            go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_FRONT, 0));
        up_vec = vec3_norm(vec3_mat4x4_prod(mat4x4_mult(\
            go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_UP, 0));
        obj_p.pos = vec3_mat4x4_prod(mat4x4_mult(\
            go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_ZERO, 1);
        obj_p.height = 1.0 * sel->transform.scale.x;
        obj_p.height *= (i) ? sel->scale[1].x : sel->scale[2].x;
        if (cube_inter(ray, obj_p, up_vec, t))
            sel->type = i;
        obj_p.dir = vec3_norm(vec3_mat4x4_prod(mat4x4_mult(\
            go_trs(sel->transform), sel->rot[i]), (t_vec3)VEC3_UP, 0));
        obj_p.pos = vec3_mat4x4_prod(mat4x4_mult(\
            go_trs(sel->transform), sel->rot[i + 3]), (t_vec3)VEC3_ZERO, 1);
        obj_p.height = 8.0 * sel->transform.scale.x * sel->scale[0].x;
        obj_p.radius = LINE_RADIUS * sel->transform.scale.x;
        //printf("line radius = %f\n", obj_p.radius);
        if (sel->type != 0 && i > 0 && cylinder_inter(ray, obj_p, t))
            sel->type = i;
        i++;
    }
}

unsigned int        handles_inter(t_env *env)
{
    t_ray   ray;
    double  t;
    int     mouse_down;

    t = T_MAX;
    mouse_down = get_mouse_ray(env, &ray, 0, 0);
    env->selection.type = -1;
    set_selection_colors(&env->selection);
    if (env->selection.mode & SCOP_TRANSLATE)
        translate_handles_inter(&env->selection, ray, &t);
    else if (env->selection.mode & SCOP_ROTATE)
        rotate_handles_inter(&env->selection, ray, &t);
    else if (env->selection.mode & SCOP_SCALE)
        scale_handles_inter(&env->selection, ray, &t);
    if (t < T_MAX)
    {
        printf("T EQUALS %f\n", t);
    }
    if (mouse_down && env->selection.type != -1)
    { 
        vec4_v3(&env->selection.colors[env->selection.type], \
            vec3_norm((t_vec3)SCOP_MDOWN));
        env->selection.active = 1;
        env->input.auto_rotate = 0;
    }
    else if (env->selection.type != -1)
        vec4_v3(&env->selection.colors[env->selection.type], \
            vec3_norm((t_vec3)SCOP_MOVER));
    return (0);
}
