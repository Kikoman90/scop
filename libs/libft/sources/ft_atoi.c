/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/02 14:35:20 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/29 14:36:54 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>//

int		ft_atoi(const char *str)
{
	int		i;
	int		s;
	int		r;

	r = 0;
	i = 0;
	s = 1;
	if (!str || ft_strlen(str) == 0)
	{
		printf("ft_atoi says nopes\n");
		return (0);
	}
	while (str[i] && str[i] >= 0 && str[i] <= 32)
		i++;
	if (str[i] && str[i] == '-')
		s = -1;
	if (str[i] && (str[i] == '+' || str[i] == '-'))
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + str[i] - '0';
		i++;
	}
	return (s * r);
}
