/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_bf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 12:33:32 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/17 16:35:37 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_both(char *s1, char *s2)
{
	free(s1);
	free(s2);
}

char		*ft_strjoin_bf(char *s1, char *s2)
{
	char	*n;
	int		i[2];

	i[0] = 0;
	while (s1[i[0]])
		i[0]++;
	i[1] = 0;
	while (s2[i[1]])
		i[1]++;
	n = (char *)malloc(sizeof(char) * (i[0] + i[1] + 1));
	i[0] = 0;
	while (s1[i[0]])
	{
		n[i[0]] = s1[i[0]];
		i[0]++;
	}
	i[1] = 0;
	while (s2[i[1]])
	{
		n[i[0] + i[1]] = s2[i[1]];
		i[1]++;
	}
	n[i[0] + i[1]] = '\0';
	free_both(s1, s2);
	return (n);
}
