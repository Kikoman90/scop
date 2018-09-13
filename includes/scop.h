/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 15:14:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/12 21:17:57 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include "../libs/libft/includes/libft.h"
# include "../libs/libmath/includes/libmath.h"
# include <time.h>
# include <errno.h>

# include <SDL2/SDL.h>
# include <OpenGL/gl3.h>

# define WIN_W 640
# define WIN_H 640

# define MALLOC_ERROR "memory allocation failure"
# define SDL_INIT_ERROR "failed to initialize sdl"
# define WIN_CREATE_ERROR "failed to create window"
# define FILE_PREFIX_ERROR "invalid prefix in file "

enum clean_flags
{
	CLEAN_SDL = 1 << 0,
	CLEAN_PARSER = 1 << 1,
	CLEAN_OBJ_LIST = 1 << 2,
	CLEAN_ALL = 1 << 3
};

typedef struct			s_parser
{
	unsigned int		fline;
	int					fsize;
	const char			*fname;
	char				*data;
}						t_parser;

typedef struct			s_camera
{
	t_mat4x4			transform;
	float				fov;
}						t_camera;

typedef struct			s_material
{
	int					id;
	const char			*name;
}						t_material;

typedef struct			s_gameobject
{
	t_mat4x4			transform;
	t_vec3				*vertices;
	unsigned int		*indices;
	t_material			mtl;
	// this is TBD (a pointer to a mtl, or a mtlID would make more sense)
	// only one instance of a material, used by all the go sharing that mat.
	const char			*name; // idk ?
}						t_gameobject;

typedef struct			s_go_node {
	t_gameobject		go;
	struct s_go_node	*next;
}						t_go_node;

typedef struct			s_env
{
	int					loop;
	SDL_Window			*window;
	SDL_GLContext		gl_context;
	t_parser			*parser;
	t_camera			camera;
	t_go_node			*obj_list;
}						t_env;

t_env					*init_scop(t_env *env, int argc, char **argv);

void					parse_file(t_env *env, const char *path);

const char				*file_name(const char *path);
int						file_size(int fd);

void					clean_scop(t_env *env, clean_flags f);

void					*log_error_null(const char *msg);
void					log_error(const char *msg);
void					log_error_free(char *msg);

#endif
