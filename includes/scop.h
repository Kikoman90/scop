/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 15:14:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/06/04 15:14:08 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SCOP_H
# define SCOP_H

# include "../libs/libft/includes/libft.h"
# include "../libs/libmath/includes/libmath.h"
# include <time.h>
# include <SDL2/SDL.h>
# include <OpenGL/gl3.h>

# define WIN_W 640
# define WIN_H 640

# define MALLOC_ERROR "memory allocation failure"
# define SDL_INIT_ERROR "failed to initialize sdl"
# define WIN_CREATE_ERROR "failed to create window"

typedef struct		s_sdl
{
	SDL_Window		*win;
	SDL_GLContext	*glContext;
}					t_sdl;

typedef struct		s_parser	
{
	const char**	errorLogs;
}					t_parser;

typedef struct		s_env
{
	int				loop; // 1 = runLoop / 0 = breakLoop and exit
	t_sdl			*sdl;
	t_parser		*parser;
}					t_env;

typedef struct		s_gameObject
{
	t_mat4x4		transformat;

}					t_gameObject;

typedef struct		s_color
{
	
}					t_color;                                                                                                                                        

void*				LogErrorNull(const char *msg);
void				LogError(const char *msg);

// what i want : 
// * close button triggers SDL_QUIT 🤠 event
// * import button for .obj files with import flags/toggles -> texture, material, animation, rigging, normals etc.
// * input handler !
// * showFPS
// * float field and toggle for fps capping (e.g. toggle[/] and [60fps])
// * 3d textures and tetrahedrons eventually
// * error logger

// vector / math libraries in c++ with operators lh +(rh) lh =(rh)

#endif