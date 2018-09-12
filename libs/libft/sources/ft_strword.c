/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strword.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 17:39:13 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/12 17:41:47 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strword(char *s, unsigned int *offset)
{
	char	*word;

	while (ft_isblank(*s) && (*s != '\0' && *s != '\n'))
	{
		*offset++;
		s++;
	}
	word = ft_strsub(s, 0, word_length(s));
	return (word);
}

static int	word_length(char *s)
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
