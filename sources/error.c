/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 14:20:34 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/12 16:44:45 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void		log_error(const char *msg)
{
	char	*error_msg;

	error_msg = ft_strjoin("ERROR: ", msg);
	ft_putendl(error_msg);
	free(error_msg);
}

void		log_error_free(char *msg)
{
	char	*error_msg;

	error_msg = ft_strjoin("ERROR: ", msg);
	ft_putendl(error_msg);
	free(msg);
	free(error_msg);
}

void		*log_error_null(const char *msg)
{
	char	*error_msg;

	error_msg = ft_strjoin("ERROR: ", msg);
	ft_putendl(error_msg);
	free(error_msg);
	return (NULL);
}

void		parser_error(const char *err, const char *fname, unsigned int fline)
{
	char	*line_number;
	char	*error_msg;

	line_number = ft_strjoin_lf(ft_strjoin_rf(" (line ", ft_itoa(fline)), ")");
	error_msg = ft_strjoin_bf(ft_strjoin(err, "'"), \
		ft_strjoin_bf(ft_strjoin(fname, "'"), line_number));
	log_error_free(error_msg);
}

GLuint		shader_error(const char *shader_name, char *info_log, GLenum shader_type)
{
	char	*error_msg;
	char	*log;

	(void)shader_type;
	log = ft_strjoin("\n", info_log);
	error_msg = ft_strjoin_bf(ft_strjoin(SHADER_INIT_ERROR, " ("), \
		ft_strjoin_bf(ft_strjoin(shader_name, ")"), log));
	log_error_free(error_msg);
	return (0);
}
