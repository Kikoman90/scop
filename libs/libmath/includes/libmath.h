/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myMath.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 14:02:53 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/26 14:54:13 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBMATH_H
# define LIBMATH_H

# include <math.h>

# define IDENTITY_MATRIX3 {\
	{1.0f, 0.0f, 0.0f,\
	0.0f, 1.0f, 0.0f,\
	0.0f, 0.0f, 1.0f}}

# define IDENTITY_MATRIX4 {\
	{1.0f, 0.0f, 0.0f, 0.0f,\
	0.0f, 1.0f, 0.0f, 0.0f,\
	0.0f, 0.0f, 1.0f, 0.0f,\
	0.0f, 0.0f, 0.0f, 1.0f}}

typedef struct	s_vec3
{
	float		x;
	float		y;
	float		z;
}				t_vec3;

typedef struct  s_vec4
{
    float       x;
    float       y;
    float       z;
    float       w;
}               t_vec4;

typedef union   u_mat4 {
    float       m[16];
    t_vec4      v[4];
}               t_mat4;

typedef struct  s_mat4x4
{
    t_mat4      mat;
}               t_mat4x4;

typedef struct  s_quaternion
{
    float       w;
    t_vec3      v;
}               t_quaternion;

t_vec3          vec3_init_f(float f);
t_vec3          vec3_init_xyz(float x, float y, float z);

t_vec4			vec4_init_f(float f);
t_vec4			vec4_init_xyzw(float x, float y, float z, float w);
t_vec3			vec4_init_v3w(t_vec3 v3, float w);

t_vec3          vec3_scale(t_vec3 v, float s);
t_vec3          vec3_add(t_vec3 a, t_vec3 b);
t_vec3          vec3_sub(t_vec3 a, t_vec3 b);
float           vec3_length(t_vec3 v);

t_vec3          vec3_cross(t_vec3 a, t_vec3 b);
t_vec3          vec3_norm(t_vec3 v);
float           vec3_dot(t_vec3 a, t_vec3 b);



t_mat4x4        mat4x4_init();
t_mat4x4		mat4x4_init_trs(t_vec3 t, t_vec3 r, t_vec3 s);
//t_mat4x4        mat_init_f(t_mat4x4 mat, float f);
t_mat4x4        mat4x4_add(t_mat4x4 a, t_mat4x4 b);
t_mat4x4        mat4x4_sub(t_mat4x4 a, t_mat4x4 b);
t_mat4x4		mat4x4_translate(t_mat4x4, t_vec3 t);
t_mat4x4		mat4x4_transpose(t_mat4x4 mat);
t_mat4x4		quat_to_mat4x4(t_quaternion q);

t_quaternion    quat_init(float theta, t_vec3 v);
//t_quaternion    quat_inv(t_quaternion q);
t_quaternion    quat_mult(t_quaternion a, t_quaternion b);
t_vec3          vector_rot(t_quaternion q, t_vec3 v);



#endif
