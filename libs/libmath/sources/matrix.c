/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 19:23:53 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/10 17:51:44 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

	/* MEMO (RM: row-major / CM: column-major)
	
	CM | 1 0 0 2 | | 1 0 0  0 | -> | 1 0 0  2 |
	   | 0 1 0 3 | | 0 1 0  4 |    | 0 1 0  7 |
	   | 0 0 1 1 | | 0 0 1 -2 |    | 0 0 1 -1 |
	   | 0 0 0 1 | | 0 0 0  1 |    | 0 0 0  1 |

	RM | 1 0 0 0 | | 1 0  0 0 | -> | 1 0  0 0 |
	   | 0 1 0 0 | | 0 1  0 0 |    | 0 1  0 0 |
	   | 0 0 1 0 | | 0 0  1 0 |    | 0 0  1 0 |
	   | 2 3 1 1 | | 0 4 -2 1 |    | 2 7 -1 1 |

	RM : v[0] = {1 0 0 2}
	CM : v[0] = {1 0 0 2};
	tout est dans l'interpretation

	-------------------------------------------------
	
	(T)| 1 3 2 | (R)| 7   4 3 | (S)| 1 2 1 |
	   | 7 5 0 |    | 0.5 4 1 |    | 3 2 1 |
	   | 2 1 2 |    | 7   0 5 |    | 2 2 2 |
	
	(1) (tr)s;
	(2) t(rs);

	(1) | 22.5 16 16 | | 1 2 1 | -> | 102.5 109 70.5  |
	    | 51.5 48 26 | | 3 2 1 |    | 247.5 251 151.5 |
		| 28.5 12 17 | | 2 2 2 |    | 98.5  115 74.5  |

	(2) | 1 3 2 | | 25   28 17  | -> | 102.5 109 70.5  |
	    | 7 5 0 | | 14.5 11 6.5 |    | 247.5 251 151.5 |
		| 2 1 2 | | 17   24 17  |    | 98.5  115 74.5  |
	
	-------------------------------------------------

	column-major : v' = TRSv (first scale, then rotate, then translate);
	row-major    : v' = vSRT (idem) */

t_mat4x4	mat4x4(void)
{
	return ((t_mat4x4)IDENTITY_MATRIX4);
}

	/* TRS :
	
	1st technique: compute the 3 matrices and multiply them in the right order;
	2nd technique: compute all together with formula below (if scaling is uniform)

	| 1 0 0 tx | | r00 r10 r20 0 | | sx 0  0  0 | 
	| 0 1 0 ty | | r01 r11 r21 0 | | 0  sy 0  0 |
	| 0 0 1 tz | | r02 r12 r22 0 | | 0  0  sz 0 |
    | 0 0 0  1 | |  0   0   0  1 | | 0  0  0  1 |
	=
	| 1 0 0 tx | | r00sx r10sy r20sz 0 |
	| 0 1 0 ty | | r01sx r11sy r21sz 0 |
	| 0 0 1 tz | | r02sx r12sy r22sz 0 |
 	| 0 0 0  1 | |   0     0     0   1 |
	=
	| r00sx           				r10sy						  r20sz							tx |
	| r01sx           				r11sy						  r21sz							ty |
	| r02sx           				r12sy						  r22sz							tz |
	| (txr00sx + tyr01sx + tzr02sx) (txr10sy + tyr11sy + tzr12sy) (txr20sz + tyr21sz + tzr22sz) 1  |
	
	indices:
	| 0 4  8 12 |
	| 1 5  9 13 |
	| 2 6 10 14 |
	| 3 7 11 15 | */

t_mat4x4	mat4x4_trs(t_vec3 t, t_quaternion r, t_vec3 s)
{
	t_mat4x4	trs;
	t_mat4x4	rot_mat;
	t_mat4x4	pos_mat;
	t_mat4x4	sca_mat;

	rot_mat = quat_to_mat4x4(r);
	trs = (t_mat4x4)IDENTITY_MATRIX4;

	//
	pos_mat = (t_mat4x4)IDENTITY_MATRIX4;
	pos_mat.m[12] = t.x;
	pos_mat.m[13] = t.y;
	pos_mat.m[14] = t.z;
	sca_mat = (t_mat4x4)IDENTITY_MATRIX4;
	sca_mat.m[0] = s.x;
	sca_mat.m[5] = s.y;
	sca_mat.m[10] = s.z;

	trs = mat4x4_mult(rot_mat, sca_mat);
	trs = mat4x4_mult(pos_mat, trs);
	//

	// wrong order
	/*trs.m[0] = s.x * rot_mat.m[0];
	trs.m[1] = s.x * rot_mat.m[1];
	trs.m[2] = s.x * rot_mat.m[2];
	trs.m[3] = t.x;// * trs.m[0] + t.y * trs.m[1] + t.z * trs.m[2];
	trs.m[4] = s.y * rot_mat.m[4];
	trs.m[5] = s.y * rot_mat.m[5];
	trs.m[6] = s.y * rot_mat.m[6];
	trs.m[7] = t.x * trs.m[4] + t.y * trs.m[5] + t.z * trs.m[6];
	trs.m[8] = s.z * rot_mat.m[8];
	trs.m[9] = s.z * rot_mat.m[9];
	trs.m[10] = s.z * rot_mat.m[10];
	trs.m[11] = t.x * trs.m[8] + t.y * trs.m[9] + t.z * trs.m[10];
	return (mat4x4_transpose(trs));*/
	

	return (trs);
}