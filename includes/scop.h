/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 15:14:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/17 17:09:11 by fsidler          ###   ########.fr       */
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
# define MTL_UNSPECIFIED "cannot use material without loading .mtl file first"

# define GO_NAME "gameobject_"
# define MTL_NAME "material_"

typedef enum			e_clean_flags
{
	CLEAN_SDL = 1 << 0,
	CLEAN_ALL = 1 << 1
}						t_clean_flags;

typedef enum			e_seed_flags
{
	VERTEX_SEED = 1 << 0,
	INDEX_SEED = 1 << 1
}						t_seed_flags;

typedef struct			s_seed
{
	int					beginseed;
	int					endseed;
	size_t				count;
}						t_seed;

typedef struct			s_parser
{
	unsigned int		fline;
	size_t				fsize;
	char				*fpath;
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
}
						t_mtl_node;
typedef struct			s_go_node
{
	t_gameobject		*go;
	struct s_go_node	*next;
}						t_go_node;

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

/* setup.c				=> 4 functions */
void					parse_file(t_env *env, const char *path, void (*ft_parsing)(t_env*, t_parser*, int));
t_env					*init_scop(t_env *env, int argc, char **argv);

/* obj_parser.c			=> IDK YET YEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEET*/ //lel
void					parse_wavefrontobj(t_env *env, t_parser *parser, int fseed, t_go_node *bound_go);

/* mtl_parser.c			=> IDK YET YEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEET*/ //lel
void					parse_wavefrontmtl(t_env *env, t_parser *parser, int fseed, t_mtl_node *bound_mtl);

/* utils_parser.c		=> 5 functions */
t_vec3      			vec3_atof(char *data, int *seed);
int	        			skip_line(char *data, int seed);
int         			check_idx_count(char *data, int seed, int idx);
void					init_seeds(t_seed *vtx_seed, t_seed *idx_seed);
char					*generate_name(const char *typename, size_t count);

/* node_create.c		=> 4 functions */
t_go_node				*create_go_node(char *name, unsigned int mtl_id, size_t vc, size_t ic);
t_mtl_node				*create_mtl_node(char *name, unsigned int id);

/* node_add.c			=> 2 functions */
t_go_node				*add_go_node(t_go_node *dest, t_go_node *node);
t_mtl_node				*add_mtl_node(t_mtl_node *dest, t_mtl_node *node);

/* clean.c				=> 4 functions */
void					clean_go_node(t_go_node *node);
void					clean_mtl_node(t_mtl_node *node);

/* cleanup.c			=> 4 functions */
void					*clean_scop(t_env *env, t_clean_flags f);

/* error.c				=> 4 functions */
void					log_error(const char *msg);
void					log_error_free(char *msg);
void					*log_error_null(const char *msg);
void					prefix_error(const char *fname, unsigned int fline);

/* display.c -> to be removed */
//
void					display_gameobject(t_gameobject *obj);
void					display_go_list(t_go_node *list);
//

#endif
