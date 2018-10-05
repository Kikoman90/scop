/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 15:14:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/04 20:27:19 by fsidler          ###   ########.fr       */
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
# define SHADER_INIT_ERROR "shader initialization failed"

# define GO_NAME "gameobject_"
# define MTL_NAME "material_"

typedef enum			e_clean_flags
{
	CLEAN_SDL = 1 << 0,
	CLEAN_ALL = 1 << 1
}						t_clean_flags;

typedef struct			s_transform
{
	t_vec3				position;
	t_quaternion		rotation;
	t_vec3				scale;
}						t_transform;

typedef struct			s_gl_stack
{
	GLuint				vao;
	GLuint				vbo;
	GLuint				ibo;
}						t_gl_stack;

typedef struct			s_shader
{
	const char			*name;
	GLuint				vtx_s;
	GLuint				frg_s;
	GLuint				prog;
}						t_shader;

typedef struct			s_material
{
	char				*name;
	t_vec3				clr_amb;
	t_vec3				clr_dif;
	t_vec3				clr_spc;
	float				expnt_spc;
	float				transparency;
}						t_material;

typedef struct			s_gameobject
{
	t_transform			transform;
	t_vec3				*vertices;
	size_t				vtx_count;
	unsigned int		*indices;
	size_t				idx_count;
	unsigned int		mtl_id;
	char				*name;
	t_gl_stack			*gl_stack;
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
	t_transform			transform;
	float				fov;
	float				znear;
	float				zfar;
}						t_camera;

typedef struct			s_env
{
	int					loop;
	SDL_Window			*window;
	SDL_GLContext		gl_context;
	t_camera			camera;
	t_mat4x4			view_mat;
	t_mat4x4			proj_mat;
	t_shader			def_shader;
	t_go_node			*go_list;
	t_mtl_node			*mtl_list;
	size_t				go_count;
	size_t				mtl_count;
}						t_env;

/*
** setup.c				=> ? functions
*/
void					parse_file(t_env *env, const char *path, void \
									(*ft_parsing)(t_env*, t_parser*, char*));
t_env					*init_scop(t_env *env, int argc, char **argv);

/*
** obj_parser.c			=> ? functions
*/
void					parse_wavefrontobj(t_env *env, t_parser *parser, \
											char *word);

/*
** mtl_parser.c			=> ? functions
*/
void					parse_wavefrontmtl(t_env *env, t_parser *parser, \
										char *word);

unsigned int			get_mtl_id(t_env *env, char *mtl_name, unsigned int mtl_offset);

void					gl_stack_feed(t_gameobject *go);
t_go_node				*create_go_node(char *name, unsigned int mtl_id, \
										size_t vc, size_t ic);
t_mtl_node				*create_mtl_node(char *name);

t_go_node				*add_go_node(t_env *env, t_go_node *node);
t_mtl_node				*add_mtl_node(t_env *env, t_mtl_node *node);

void					clean_go_node(t_go_node *node);
void					clean_mtl_node(t_mtl_node *node);

void					*clean_scop(t_env *env, t_clean_flags f);

/*
** error.c				=> 5 functions
*/
void					log_error(const char *msg);
void					log_error_free(char *msg);
void					*log_error_null(const char *msg);
void					prefix_error(const char *fname, unsigned int fline);
GLuint					shader_error(const char *shader_name, char *info_log, \
										GLenum shader_type);

//
void					display_vec3(const char *p, t_vec3 v);
void					display_gameobject(t_gameobject *obj);
void					display_go_list(t_go_node *list);
void					display_material(t_material *mtl);
void					display_mtl_list(t_mtl_node *list);
//

#endif
