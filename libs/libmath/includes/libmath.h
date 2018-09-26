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

# define MAT_ID 0x7FFFFFFF

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

t_vec3          vec_init_f(float f);
t_vec3          vec_init_xyz(float x, float y, float z);

t_vec3          vec_scale(t_vec3 v, float s);
t_vec3          vec_add(t_vec3 a, t_vec3 b);
t_vec3          vec_sub(t_vec3 a, t_vec3 b);
float           vec_length(t_vec3 v);

t_vec3          vec_cross(t_vec3 a, t_vec3 b);
t_vec3          vec_norm(t_vec3 v);
float           vec_dot(t_vec3 a, t_vec3 b);

t_mat4x4        mat_init_f(t_mat4x4 mat, float f);
t_mat4x4        mat_add(t_mat4x4 a, t_mat4x4 b);
t_mat4x4        mat_sub(t_mat4x4 a, t_mat4x4 b);

/*t_quaternion    quat_init(float theta, t_vec3 v);
t_quaternion    quat_inv(t_quaternion q);
t_quaternion    quat_mult(t_quaternion a, t_quaternion b);
t_vec3          vector_rot(t_quaternion q, t_vec3 v);*/



#endif
