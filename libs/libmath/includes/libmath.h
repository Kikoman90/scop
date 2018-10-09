/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libmath.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 14:02:53 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/09 17:39:41 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBMATH_H
# define LIBMATH_H

# include <math.h>

# define VEC3_ZERO {{0.0f, 0.0f, 0.0f}}
# define VEC3_UP {{0.0f, 1.0f, 0.0f}}
//# define VEC3_ONE {{1.0f, 1.0f, 1.0f}}
//# define VEC3_RIGHT {{1.0f, 0.0f, 0.0f}}
//# define VEC3_FRONT {{0.0f, 0.0f, 1.0f }} // OR -1 ?? comon man

# define VEC4_ZERO {{0.0f, 0.0f, 0.0f, 0.0f}}

# define IDENTITY_MATRIX3 {\
	{1.0f, 0.0f, 0.0f,\
	 0.0f, 1.0f, 0.0f,\
	 0.0f, 0.0f, 1.0f}}

# define IDENTITY_MATRIX4 {\
	{1.0f, 0.0f, 0.0f, 0.0f,\
	 0.0f, 1.0f, 0.0f, 0.0f,\
	 0.0f, 0.0f, 1.0f, 0.0f,\
	 0.0f, 0.0f, 0.0f, 1.0f}}

typedef union		u_vec3
{
	float			v[3];
	struct
	{
		union
		{
			float	x;
			float	r;
		};
		union
		{
			float	y;
			float	g;
		};
		union
		{
			float	z;
			float	b;
		};
	};
}					t_vec3;

typedef union		u_vec4
{
	float			v[4];
	struct
	{
		union
		{
			float	x;
			float	r;
		};
		union
		{
			float	y;
			float	g;
		};
		union
		{
			float	z;
			float	b;
		};
		union
		{
			float	w;
			float	a;
		};
	};
}					t_vec4;

typedef union		u_mat4x4 {
	float			m[16];
	t_vec4			v[4];
}					t_mat4x4;

typedef struct		s_quaternion
{
    float			w;
	union
	{
		t_vec3		v;
		struct
		{
			float	x;
			float	y;
			float	z;
		};
	};
}					t_quaternion;


//t_vec3				vec3(void);
t_vec3				vec3_f(float f);
t_vec3				vec3_xyz(float x, float y, float z);

//t_vec4				vec4(void);
t_vec4				vec4_f(float f);
t_vec4				vec4_xyzw(float x, float y, float z, float w);
t_vec4				vec4_v3w(t_vec3 v3, float w);

//float       		vec4_dot(t_vec4 a, t_vec4 b);

t_vec3				vec3_scale(t_vec3 v, float s);
t_vec3				vec3_add(t_vec3 a, t_vec3 b);
t_vec3				vec3_sub(t_vec3 a, t_vec3 b);
float				vec3_length(t_vec3 v);

t_vec3				vec3_cross(t_vec3 a, t_vec3 b);
t_vec3				vec3_norm(t_vec3 v);
float				vec3_dot(t_vec3 a, t_vec3 b);


t_mat4x4			mat4x4(void);
//t_mat4x4			mat4x4_f(float f);
t_mat4x4			mat4x4_trs(t_vec3 t, t_quaternion r, t_vec3 s);

t_mat4x4			mat4x4_add(t_mat4x4 a, t_mat4x4 b);
t_mat4x4			mat4x4_sub(t_mat4x4 a, t_mat4x4 b);
t_mat4x4			mat4x4_translate(t_vec3 t);
t_mat4x4			mat4x4_scale(t_vec3 s);
t_mat4x4			mat4x4_transpose(t_mat4x4 mat);

t_vec4				vec4_mat4x4_prod(t_vec4 v, t_mat4x4 mat);
t_vec3				vec3_mat4x4_prod(t_vec3 v, t_mat4x4 mat);

t_mat4x4			mat4x4_mult(t_mat4x4 a, t_mat4x4 b);

t_mat4x4			quat_to_mat4x4(t_quaternion q);


t_quaternion		quat(void);
t_quaternion		quat_tv(float theta, t_vec3 v);
t_quaternion		quat_norm(t_quaternion q);
t_quaternion		quat_mult(t_quaternion a, t_quaternion b);

t_vec3				vector_rot(t_quaternion q, t_vec3 v);

//t_quaternion    quat_inv(t_quaternion q);

#endif
