/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/19 18:34:21 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

// void __attribute__((constructor)) begin(); //remove after checking leaks
// void __attribute__((destructor)) end(); // remove after checking leaks

/*void		update_objects(t_env *env, double delta)
{
	//if (SDL_MOUSE_BUTTON(1))

}*/

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
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (env->input.auto_rotate)
			rotate_gameobjects(env->gameobjects.head, env->delta_time);
		if (env->input.fade_coef)
		{
			if ((env->input.fade = ft_fclamp(env->input.fade + \
				env->input.fade_coef * env->delta_time, 0, 1)) == 1 ||\
				env->input.fade == 0)
				env->input.fade_coef = 0;
		}
		draw(env);
		//glBindFramebuffer(GL_FRAMEBUFFER, env->pick_fbo);
		//handle_picking() . picking when you handle mouse events ?
		SDL_GL_SwapWindow(env->win_env.window);
		handle_events_and_input(env);
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
