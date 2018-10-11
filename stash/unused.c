/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unused.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 16:32:17 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/11 16:39:54 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_mat4x4	look_at(t_vec3 pos, t_vec3 target, t_vec3 up)
{
	t_vec3		xaxis;
	t_vec3		yaxis;
	t_vec3		zaxis;
	t_mat4x4	lookat;
	
	zaxis = vec3_norm(vec3_sub(pos, target));
	xaxis = vec3_norm(vec3_cross(up, zaxis));
	yaxis = vec3_cross(zaxis, xaxis);

	lookat = mat4x4();
	lookat.v[0] = vec4_xyzw(xaxis.x, yaxis.x, zaxis.x, 0);
	lookat.v[1] = vec4_xyzw(xaxis.y, yaxis.y, zaxis.y, 0);
	lookat.v[2] = vec4_xyzw(xaxis.z, yaxis.z, zaxis.z, 0);
	lookat.v[3] = vec4_xyzw(-vec3_dot(xaxis, pos), -vec3_dot(yaxis, pos)\
								, -vec3_dot(zaxis, pos), 1);
	return (lookat);
}

