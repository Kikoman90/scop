/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/03 18:30:32 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/*
** void __attribute__((constructor)) begin(); //remove after checking leaks
** void __attribute__((destructor)) end(); // remove after checking leaks
*/

static void	loop(t_env *env)
{
	uint32_t		last_time;
	uint32_t		cur_time;

	cur_time = SDL_GetTicks();
	while (env->loop == 1)
	{
		last_time = cur_time;
		cur_time = SDL_GetTicks();
		env->delta_time = (cur_time - last_time) / 1000.0;
		handle_events_and_input(env);
		if (env->input.auto_rotate)
		{
			rotate_gameobjects(env->gameobjects.head, env->delta_time);
			set_selection_transform(&env->selection, env->selection.localspace);
		}
		if (env->input.fade_coef && ((env->input.fade = ft_fclamp(\
			env->input.fade + env->input.fade_coef * env->delta_time, 0, 1)) \
			== 1 || env->input.fade == 0))
			env->input.fade_coef = 0;
		draw(env);
		SDL_GL_SwapWindow(env->win_env.window);
	}
}

int			main(int argc, char **argv)
{
	t_env	env;

	if (!init_scop(&env, argc, argv))
		return (clean_scop(&env));
	loop(&env);
	return (clean_scop(&env));
}
