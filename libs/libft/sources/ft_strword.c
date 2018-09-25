/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strword.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 17:39:13 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/17 16:35:06 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_length(char const *s)
{
	int	len;

	len = 0;
	while (*s && !ft_isblank(*s) && *s != '\0' && *s != '\n')
	{
		len++;
		s++;
	}
	return (len);
}

char			*ft_strword(char const *s, unsigned int *offset)
{
	char	*word;
	size_t	wsize;

	s += *offset;
	word = NULL;
	while (*s && ft_isblank(*s) && (*s != '\0' && *s != '\n'))
	{
		*offset += 1;
		s++;
	}
	wsize = word_length(s);
	*offset += wsize;
	if (wsize > 0)
		word = ft_strsub(s, 0, wsize);
	return (word);
}
