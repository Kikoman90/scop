/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 15:14:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/25 16:34:04 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include <time.h>
# include <errno.h>

# include "parser.h"

# include <SDL2/SDL.h>
# include <OpenGL/gl3.h>

# define WIN_W 640
# define WIN_H 640

# define MALLOC_ERROR "memory allocation failure"
# define SDL_INIT_ERROR "failed to initialize sdl"
# define WIN_CREATE_ERROR "failed to create window"

# define GO_NAME "gameobject_"
# define MTL_NAME "material_"

typedef enum			e_clean_flags
{
	CLEAN_SDL = 1 << 0,
	CLEAN_ALL = 1 << 1
}						t_clean_flags;

typedef struct			s_material
{
	unsigned int		id;
	char				*name;
}						t_material;

typedef struct			s_gameobject
{
	t_mat4x4			transform;
	t_vec3				*vertices;
	size_t				vtx_count;
	unsigned int		*indices;
	size_t				idx_count;
	unsigned int		mtl_id;
	char				*name;
}						t_gameobject;

typedef struct			s_mtl_node
{
	t_material			*mtl;
	struct s_mtl_node	*next;
}						t_mtl_node;

typedef struct			s_go_node
{
	t_gameobject		*go;
	struct s_go_node	*next;
}						t_go_node;

typedef struct			s_camera
{
	t_mat4x4			transform;
	float				fov;
}						t_camera;

typedef struct			s_env
{
	int					loop;
	SDL_Window			*window;
	SDL_GLContext		gl_context;
	t_camera			camera;
	t_go_node			*go_list;
	t_mtl_node			*mtl_list;
	size_t				go_count;
	size_t				mtl_count;
}						t_env;

void					parse_file(t_env *env, const char *path, void \
									(*ft_parsing)(t_env*, t_parser*, char*));
t_env					*init_scop(t_env *env, int argc, char **argv);

void					parse_wavefrontobj(t_env *env, t_parser *parser, \
										char *word);

void					parse_wavefrontmtl(t_env *env, t_parser *parser, \
										char *word);

t_go_node				*create_go_node(char *name, unsigned int mtl_id, \
										size_t vc, size_t ic);
t_mtl_node				*create_mtl_node(char *name, unsigned int id);

t_go_node				*add_go_node(t_env *env, t_go_node *node);
t_mtl_node				*add_mtl_node(t_env *env, t_mtl_node *node);

void					clean_go_node(t_go_node *node);
void					clean_mtl_node(t_mtl_node *node);

void					*clean_scop(t_env *env, t_clean_flags f);

void					log_error(const char *msg);
void					log_error_free(char *msg);
void					*log_error_null(const char *msg);
void					prefix_error(const char *fname, unsigned int fline);

//
void					display_gameobject(t_gameobject *obj);
void					display_go_list(t_go_node *list);
//

#endif
