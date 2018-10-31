/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 15:25:41 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/30 21:52:12 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c, int inc)
{
	if (!str)
		return (NULL);
	while (*str && *str != (UC)c)
		str++;
	if (*str && *str == (UC)c)
	{
		if (inc == 1)
			str++;
		if (*str)
			return ((char *)str);
	}
	return (NULL);
}

char	*ft_strnchr(const char *str, int c, int n, int inc)
{
	while (n--)
	{
		if (str)
			str = ft_strchr(str, c, inc);
	}
	return ((char *)str);
}
