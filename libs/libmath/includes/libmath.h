/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myMath.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 14:02:53 by fsidler           #+#    #+#             */
/*   Updated: 2018/06/11 14:02:54 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBMATH_H
# define LIBMATH_H

# include <math.h>

# define M_PI 3.14159265358979323846
# define MAT_ID 0x7FFFFFFF

typedef struct  s_mat4x4
{
    float       m[16];
    /*float       m00;
    float       m01;
    float       m02;
    float       m03;
    float       m10;
    float       m11;
    float       m12;
    float       m13;    
    float       m20;
    float       m21;
    float       m22;
    float       m23;
    float       m30;
    float       m31;
    float       m32;
    float       m33;*/
}               t_mat4x4;

typedef struct  s_vec3
{
    float       x;
    float       y;
    float       z;
}               t_vec3;

typedef struct  s_vec4
{
    float       x;
    float       y;
    float       z;
    float       w;
}               t_vec4;

typedef struct  s_quaternion
{
    float       w;
    t_vec3      v;
}               t_quaternion;

t_vec3          vecInit(float x, float y, float z);
t_vec3          vecScale(t_vec3 v, float s);
t_vec3          vecAdd(t_vec3 a, t_vec3 b);
t_vec3          vecSub(t_vec3 a, t_vec3 b);
float           vecLength(t_vec3 v);

t_vec3          vecCross(t_vec3 a, t_vec3 b);
t_vec3          vecNorm(t_vec3 v);
float           vecDot(t_vec3 a, t_vec3 b);

t_quaternion    quatInit(float theta, t_vec3 v);
t_quaternion    quatInv(t_quaternion q);
t_quaternion    quatMult(t_quaternion a, t_quaternion b);
t_vec3          vectorRot(t_quaternion q, t_vec3 v);



#endif