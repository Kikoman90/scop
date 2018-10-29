/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 12:23:46 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/29 19:34:45 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

unsigned int	get_mtl_id(t_env *env, char *mtl_name, unsigned int mtl_offset)
{
	t_mtl_node		*mtl_list;

	mtl_list = env->mtl_list;
	while (mtl_list)
	{
		if (mtl_list->id > mtl_offset && \
			ft_strcmp(mtl_name, mtl_list->mtl->name) == 0)
		{
			free(mtl_name);
			return (mtl_list->id);
		}
		mtl_list = mtl_list->next;
	}
	free(mtl_name);
	return (0);
}


// -> to libmath (not so sure about that)
t_vec3			vec3_atof(char *data, unsigned int *seed, int clamp01)
{
	t_vec3	res;

	res.x = ft_atof_f(ft_strword(data, seed));
	res.y = ft_atof_f(ft_strword(data, seed));
	res.z = ft_atof_f(ft_strword(data, seed)); // = 1.0f;
	if (clamp01 == 1)
	{
		res.x = ft_fclamp(res.x, 0, 1);
		res.y = ft_fclamp(res.y, 0, 1);
		res.z = ft_fclamp(res.z, 0, 1);
	}
	return (res);
}

unsigned int	skip_line(char *data, unsigned int seed)
{
	while (data[seed] && data[seed] != '\n')
		seed += 1;
	if (data[seed] && data[seed] == '\n')
		seed += 1;
	return (seed);
}

unsigned int	check_idx_count(char *data, unsigned int seed, int idx)
{
	unsigned int count;

	count = 0;
	while (data[seed] != '\n' && data[seed] != '\0' && \
		(seed = ft_wordoffset(data, seed)))
		count++;
	if (idx == 1 && count > 3)
		count = 3 + (count - 3) * 3;
	return (count);
}
