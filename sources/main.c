/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 19:14:08 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/05 18:14:42 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

// void __attribute__((constructor)) begin(); //remove after checking leaks
// void __attribute__((destructor)) end(); // remove after checking leaks

/*void		add_to_selection(t_go_node *selection, t_go_node *go_list, \
	size_t *count, unsigned int id)
{
	t_go_node	*tmp;

	tmp = selection;
	if (tmp == NULL && (selection = get_go_node(go_list, id)))
		*count += 1;
	else
	{
		while (tmp->next != NULL)
		{
			if (id == tmp->id)
				return ;
			tmp = tmp->next;
		}
		if (id != tmp->id && (tmp->next = get_go_node(go_list, id)))
			*count += 1;
	}
}*/

/*void		handle_picking(t_go_node *selection, t_go_node *go_list, \
	size_t *count)
{
	int				x;
	int				y;
	unsigned int	picked_id;
	unsigned char	data[3];

	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(1))
	{
		glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
		picked_id = data[0] + data[1] * 256 + data[2] * 256 * 256;
		// if hold shift -> add
		// else clear, and add (if picked_id != 0)
		if (picked_id != 0)
			add_to_selection(selection, go_list, count, picked_id);
	}
}*/

/*void		update_objects(t_env *env, double delta)
{
	//if (SDL_MOUSE_BUTTON(1))

}*/

static void	loop(t_env *env)
{
	uint32_t	last_time;
	uint32_t	cur_time;
	double		delta_time;

	cur_time = SDL_GetTicks();
	while (env->loop == 1)
	{
		last_time = cur_time;
		cur_time = SDL_GetTicks();
		delta_time = (cur_time - last_time) / 1000.0;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		handle_events_and_input(env, delta_time);
		//update_objects(env, delta_time);
		rotate_gameobjects(env->gameobjects.head, delta_time);
		draw(env);
		SDL_GL_SwapWindow(env->win_env.window);
	}
}

int			main(int argc, char **argv)
{
	t_env	env;

	if (!init_scop(&env, argc, argv))
		return (clean_scop(&env));
	// display_go_list(env.gameobjects.head);
	loop(&env);
	return (clean_scop(&env));
}
