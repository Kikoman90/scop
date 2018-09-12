/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strword.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 17:39:13 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/12 18:12:30 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_length(char *s)
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

char			*ft_strword(char *s, int *offset)
{
	char	*word;
	size_t	wsize;

	s += *offset;
	if (*s != '\0' && *s == '\n')
	{
		s++;
		*offset += 1;
	}
	while (ft_isblank(*s) && (*s != '\0' && *s != '\n'))
	{
		*offset += 1;
		s++;
	}
	wsize = word_length(s);
	word = ft_strsub(s, 0, wsize);
	*offset += wsize;
	return (word);
}
