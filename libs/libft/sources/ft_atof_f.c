/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof_f.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 12:45:28 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/17 16:32:54 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	init_value(t_info *env)
{
	env->a = 0.0;
	env->e = 0;
	env->sign = 1;
	env->i = 0;
	env->count = 0;
}

static void	check_power(t_info *env, const char *tmp)
{
	if (env->c == '.')
	{
		while ((env->c = *tmp++) != '\0' && ft_isdigit(env->c))
		{
			env->a = env->a * 10.0 + (env->c - '0');
			env->e = env->e - 1;
		}
	}
	if (env->c == 'e' || env->c == 'E')
	{
		env->c = *tmp++;
		if (env->c == '+')
			env->c = *tmp++;
		else if (env->c == '-')
		{
			env->c = *tmp++;
			env->sign = -1;
		}
		while (ft_isdigit(env->c))
		{
			env->i = env->i * 10 + (env->c - '0');
			env->c = *tmp++;
		}
		env->e += env->i * env->sign;
	}
}

float		find_float(t_info *env)
{
	while (env->e > 0)
	{
		env->a *= 10.0;
		env->e--;
	}
	while (env->e < 0)
	{
		env->a *= 0.1;
		env->e++;
	}
	if (env->count == 1)
		env->a = env->a * -1;
	return (env->a);
}

float		ft_atof_f(char *s)
{
	char	*tmp;
	t_info	*env;
	float	res;

	if ((env = malloc(sizeof(t_info))) == NULL)
		return (0);
	init_value(env);
	tmp = s;
	while (*tmp && (*tmp == ' ' || *tmp == '\t'))
		tmp++;
	if (*tmp == '-' && *tmp++)
		env->count = 1;
	while ((env->c = *tmp++) != 0 && ft_isdigit(env->c))
		env->a = env->a * 10.0 + (env->c - '0');
	check_power(env, tmp);
	res = find_float(env);
	free(env);
	free(s);
	return (res);
}
