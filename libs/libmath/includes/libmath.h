/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libmath.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 14:02:53 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/29 15:00:38 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBMATH_H
# define LIBMATH_H

# include <math.h>

# define VEC3_ZERO {0.0f, 0.0f, 0.0f}
# define VEC3_ONE {1.0f, 1.0f, 1.0f}
# define VEC3_UP {0.0f, 1.0f, 0.0f}
# define VEC3_RIGHT {1.0f, 0.0f, 0.0f}
# define VEC3_FRONT {0.0f, 0.0f, 1.0f}
# define VEC3_DOWN {0.0f, -1.0f, 0.0f}

# define VEC4_ZERO {0.0f, 0.0f, 0.0f, 0.0f}
# define VEC4_ONE {1.0f, 1.0f, 1.0f, 1.0f}

typedef struct		s_vec2
{
	float			x;
	float			y;
}					t_vec2;

typedef struct		s_vec3
{
	float			x;
	float			y;
	float			z;
}					t_vec3;

typedef struct		s_vec4
{
	float			x;
	float			y;
	float			z;
	float			w;
}					t_vec4;

/*
** column-major matrix
*/
typedef union		u_mat4x4 {
	float			m[16];
	t_vec4			v[4];
}					t_mat4x4;

typedef struct		s_quaternion
{
	float			w;
	t_vec3			v;
}					t_quaternion;

typedef struct		s_transform
{
	t_vec3			position;
	t_quaternion	rotation;
	t_vec3			scale;
}					t_transform;

/*
** vector.c			=> 5 functions
*/
t_vec2				vec2_f(float f);
t_vec2				vec2_xy(float x, float y);
t_vec2				vec2_v3(t_vec3 v);
t_vec3				vec3_f(float f);
t_vec3				vec3_xyz(float x, float y, float z);

/*
** vectorbis.c		=> 4 functions
*/
t_vec3				vec3_v4(t_vec4 v4);
t_vec4				vec4_f(float f);
t_vec4				vec4_xyzw(float x, float y, float z, float w);
t_vec4				vec4_v3w(t_vec3 v3, float w);
void				vec4_v3(t_vec4 *v, t_vec3 v3);

/*
** vector_basop.c	=> 4 functions
*/
t_vec3				vec3_scale(t_vec3 v, float s);
t_vec3				vec3_add(t_vec3 a, t_vec3 b);
t_vec3				vec3_sub(t_vec3 a, t_vec3 b);
float				vec3_length(t_vec3 v);

/*
** vector_advop.c	=> 3 functions
*/
t_vec3				vec3_cross(t_vec3 a, t_vec3 b);
t_vec2				vec2_norm(t_vec2 v);
t_vec3				vec3_norm(t_vec3 v);
float				vec3_dot(t_vec3 a, t_vec3 b);

/*
** matrix.c			=> 4 functions
*/
t_mat4x4			mat4x4(void);
t_mat4x4			mat4x4_trs(t_vec3 t, t_quaternion r, t_vec3 s);
t_mat4x4			compute_view(t_transform cam_transform);
t_mat4x4			compute_perspective(float fov, float aspect, float zn, \
						float zf);

/*
** matrix_basop.c	=> 5 functions
*/
t_mat4x4			mat4x4_add(t_mat4x4 a, t_mat4x4 b);
t_mat4x4			mat4x4_sub(t_mat4x4 a, t_mat4x4 b);
t_mat4x4			mat4x4_transpose(t_mat4x4 mat);
void				get_matrix_axes(t_vec3 axes[3], t_mat4x4 mat);

/*
** matrix_advop.c	=> 4 functions
*/
t_vec3				vec3_mat4x4_prod(t_mat4x4 mat, t_vec3 v, float w);
t_vec4				vec4_mat4x4_prod(t_mat4x4 mat, t_vec4 v);
t_mat4x4			mat4x4_mult(t_mat4x4 a, t_mat4x4 b);
t_mat4x4			quat_to_mat4x4(t_quaternion q);

/*
** quaternion.c		=> 5 functions
*/
t_quaternion		quat(void);
t_quaternion		quat_tv(float theta, t_vec3 v);
t_quaternion		quat_norm(t_quaternion q);
t_quaternion		quat_mult(t_quaternion a, t_quaternion b);
t_vec3				vector_rot(t_quaternion q, t_vec3 v);

/*
** transform.c		=> 3 functions
*/
t_mat4x4			go_trs(t_transform tr);
t_transform			init_transform(void);
t_transform			init_transform_trs(t_vec3 t, t_quaternion r, t_vec3 s);

#endif
