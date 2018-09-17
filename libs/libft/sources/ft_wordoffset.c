/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wordoffset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 12:45:09 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/17 16:32:13 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_length(char const *s)
{
	int	len;

	len = 0;
	while (!ft_isblank(*s) && *s != '\0' && *s != '\n')
	{
		len++;
		s++;
	}
	return (len);
}

int				ft_wordoffset(char const *s, int offset)
{
	size_t	wsize;

	s += offset;
	while (ft_isblank(*s) && (*s != '\0' && *s != '\n'))
	{
		offset += 1;
		s++;
	}
	wsize = word_length(s);
	offset += wsize;
	return (offset);
}
