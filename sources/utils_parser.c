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

t_vec3		vec3_atof(char *data, int *seed)
{
	t_vec3	res;

	res.x = ft_atof_f(ft_strword(data, seed));
	res.y = ft_atof_f(ft_strword(data, seed));
	res.z = ft_atof_f(ft_strword(data, seed));
	return (res);
}

int			skip_line(char *data, int seed)
{
	while (data[seed] && data[seed] != '\n')
		seed += 1;
	if (data[seed] && data[seed] == '\n')
		seed += 1;
	return (seed);
}

int			check_idx_count(char *data, int seed, int idx)
{
	int count;

	count = 0;
	while (data[seed] != '\n' && data[seed] != '\0' && \
		(seed = ft_wordoffset(data, seed)) > -1)
		count++;
	if (idx == 1 && count == 4)
		count = 6;
	return (count);
}

void		init_seeds(t_seed *vtx_seed, t_seed *idx_seed)
{
	vtx_seed->beginseed = -1;
	vtx_seed->endseed = -1;
	vtx_seed->count = 0;
	idx_seed->beginseed = -1;
	idx_seed->endseed = -1;
	idx_seed->count = 0;
}

char		*generate_name(char *name, const char *typename, size_t count)
{
	if (name)
		return (name);
	count++;
	return (ft_strjoin_rf(typename, ft_itoa((int)count)));
}
