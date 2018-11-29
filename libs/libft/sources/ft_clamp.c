/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clamp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 19:13:58 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/16 19:16:56 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_iclamp(int value, const int min, const int max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
	return (value);
}

float	ft_fclamp(float value, const float min, const float max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
	return (value);
}
