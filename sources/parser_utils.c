/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 12:23:46 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/31 13:17:25 by fsidler          ###   ########.fr       */
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

	count = 0;
	while (data[seed] != '\n' && data[seed] != '\0' && \
		(seed = ft_wordoffset(data, seed)))
		count++;
	if (idx == 1 && count > 3)
		count = 3 + (count - 3) * 3;
	return (count);
}

unsigned int		skip_line(char *data, unsigned int seed)
{
	while (data[seed] && data[seed] != '\n')
		seed += 1;
	if (data[seed] && data[seed] == '\n')
		seed += 1;
	return (seed);
}

static unsigned int	cmp_attrib(t_idx_attrib *att1, t_idx_attrib *att2, \
	unsigned int *ret)
{
	if (att1->attrib[0] == att2->attrib[0] && \
		att1->attrib[1] == att2->attrib[1] && \
		att1->attrib[2] == att2->attrib[2])
	{
		*ret = att1->idx_ret;
		return (1);
	}
	return (0);
}

t_idx_attrib		*check_attrib_list(t_obj_parser_var *opv, \
	t_idx_attrib *elem, unsigned int *ret)
{
	t_idx_attrib	*tmp;
	t_idx_attrib	*attrib_ret;

	tmp = opv->attrib_list;
	while (tmp && tmp->next)
	{
		if (cmp_attrib(tmp, elem, ret) == 1)
			return (tmp);
		tmp = tmp->next;
	}
	if (tmp && cmp_attrib(tmp, elem, ret) == 1)
		return (tmp);
	if (!(attrib_ret = (t_idx_attrib*)malloc(sizeof(t_idx_attrib))))
		return (log_error_null(MALLOC_ERROR));
	attrib_ret->idx_ret = opv->attrib_fill++;
	attrib_ret->attrib[0] = elem->attrib[0];
	attrib_ret->attrib[1] = elem->attrib[1];
	attrib_ret->attrib[2] = elem->attrib[2];
	attrib_ret->next = NULL;
	*ret = attrib_ret->idx_ret;
	if (tmp)
		tmp->next = attrib_ret;
	else
		opv->attrib_list = attrib_ret;
	return (attrib_ret);
}