/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 12:23:46 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/29 16:27:09 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

t_vec3				vec3_atof(char *data, unsigned int *seed, int clamp01)
{
	t_vec3	res;

	res.x = ft_atof_f(ft_strword(data, seed));
	res.y = ft_atof_f(ft_strword(data, seed));
	res.z = ft_atof_f(ft_strword(data, seed));
	if (clamp01 == 1)
	{
		res.x = ft_fclamp(res.x, 0, 1);
		res.y = ft_fclamp(res.y, 0, 1);
		res.z = ft_fclamp(res.z, 0, 1);
	}
	return (res);
}

unsigned int		check_idx_count(char *data, unsigned int seed, int idx)
{
	unsigned int	count;
	char			*word;

	count = 0;
	while (data[seed] != '\n' && data[seed] != '\0')
	{
		if ((word = ft_strword(data, &seed)))
		{
			free(word);
			count++;
		}
	}
	if (idx == 1 && count > 3)
		count = 3 + (count - 3) * 3;
	return (count);
}
