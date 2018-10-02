/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 12:23:46 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/02 19:45:34 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

unsigned int	get_mtl_id(t_env *env, char *mtl_name, unsigned int mtl_offset)
{
	unsigned int	i;
	t_mtl_node		*mtl_list;

	i = 0;
	mtl_list = env->mtl_list;
	while (mtl_list)
	{
		if (i >= mtl_offset && ft_strcmp(mtl_name, mtl_list->mtl->name) == 0)
			return (i);
		i++;
		mtl_list = mtl_list->next;
	}
	return (0);
}

// -> to libft ft_iclamp (clamp.c)
int				iclamp(int value, const int min, const int max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
	return (value);
}

// -> to libft ft_fclamp (clamp.c)
float			fclamp(float value, const float min, const float max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
	return (value);
}

// -> to libmath
t_vec3			vec3_atof(char *data, unsigned int *seed, int clamp01)
{
	t_vec3	res;

	res.x = ft_atof_f(ft_strword(data, seed));
	res.y = ft_atof_f(ft_strword(data, seed));
	res.z = ft_atof_f(ft_strword(data, seed));
	if (clamp01 == 1)
	{
		res.x = fclamp(res.x, 0, 1);
		res.y = fclamp(res.y, 0, 1);
		res.z = fclamp(res.z, 0, 1);
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
	if (idx == 1 && count == 4)
		count = 6;
	return (count);
}

char			*generate_name(char *name, const char *typename, size_t count)
{
	if (name)
		return (name);
	count++;
	return (ft_strjoin_rf(typename, ft_itoa((int)count)));
}
