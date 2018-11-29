/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 15:16:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/15 14:17:05 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	log_error(const char *msg)
{
	char	*error_msg;

	error_msg = ft_strjoin("ERROR: ", msg);
	ft_putendl(error_msg);
	free(error_msg);
	return (0);
}

unsigned int	log_error_free(char *msg)
{
	char	*error_msg;

	error_msg = ft_strjoin("ERROR: ", msg);
	ft_putendl(error_msg);
	free(error_msg);
	free(msg);
	return (0);
}

void			*log_error_null(const char *msg)
{
	char	*error_msg;

	error_msg = ft_strjoin("ERROR: ", msg);
	ft_putendl(error_msg);
	free(error_msg);
	return (NULL);
}

void			*log_error_null_free(char *msg)
{
	char	*error_msg;

	error_msg = ft_strjoin("ERROR: ", msg);
	ft_putendl(error_msg);
	free(error_msg);
	free(msg);
	return (NULL);
}
