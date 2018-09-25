/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 12:23:46 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/24 19:43:17 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

t_vec3			vec3_atof(char *data, unsigned int *seed)
{
	t_vec3	res;

	res.x = ft_atof_f(ft_strword(data, seed));
	res.y = ft_atof_f(ft_strword(data, seed));
	res.z = ft_atof_f(ft_strword(data, seed));
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
