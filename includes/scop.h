/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 15:14:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/29 13:11:42 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include <time.h>
# include <errno.h>

# include "parser.h"

# include <SDL2/SDL.h>
# include <OpenGL/gl3.h>

# define WIN_W 1080
# define WIN_H 900

# define MALLOC_ERROR "memory allocation failure"
# define SDL_INIT_ERROR "failed to initialize sdl"
# define WIN_CREATE_ERROR "failed to create window"
# define SHADER_INIT_ERROR "shader initialization failed"
# define FRAMEBUFFER_INCOMPLETE_ERROR "framebuffer imcomplete"

# define BUFFER_OFFSET (char*)NULL + (i);

# define GO_NAME "gameobject_"
# define MTL_NAME "material_"

typedef enum			e_clean_flags
{
	CLEAN_SDL_GL = 1 << 0,
	CLEAN_ALL = 1 << 1
}						t_clean_flags;

typedef enum			e_uniforms
{
	DEF_SHADER_UNIFORMS = 1 << 0,
	STD_SHADER_UNIFORMS = 1 << 1,
	PICK_SHADER_UNIFORMS = 1 << 2
}						t_uniforms;

typedef struct			s_transform
{
	t_quaternion		rotation;
	t_vec3				position;
	t_vec3				scale;
}						t_transform;

typedef struct			s_gl_stack
{
	GLuint				vao;
	GLuint				ibo;
	GLuint				vbo[4];
}						t_gl_stack;

typedef struct			s_shader
{
	const char			*name;
	GLuint				vtx_s;
	GLuint				frg_s;
	GLuint				prog;
	GLint				u_loc[11];
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

typedef struct			s_vtx_attrib
{
	t_vec2				uv;
	t_vec3				vertex;
	t_vec3				normal;
	t_vec3				color;
}						t_vtx_attrib;

typedef struct			s_gameobject
{
	char				*name;
	t_transform			transform;
	t_vtx_attrib		*vtx_attrib;
	unsigned int		*indices;
	size_t				vtx_count;
	size_t				idx_count;
	unsigned int		mtl_id;
	t_vec3				pick_clr;
	t_gl_stack			*gl_stack;
}						t_gameobject;

typedef struct			s_light
{
	t_gameobject		*go;
	t_vec3				light_color;
	float				intensity;
	float				range;
}						t_light;

typedef struct			s_mtl_node
{
	unsigned int		id;
	t_material			*mtl;
	struct s_mtl_node	*next;
}						t_mtl_node;

typedef struct			s_go_node
{
	unsigned int		id;
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
	GLuint				ms_fbo;
	GLuint				pick_fbo;
	t_shader			def_shader;
	t_shader			pick_shader;
	t_shader			std_shader;
	t_camera			camera;
	t_light				light;
	t_mat4x4			proj_mat;
	t_mat4x4			*go_mat;
	t_mtl_node			*mtl_list;
	t_go_node			*go_list;
	t_go_node			*selection;
	size_t				mtl_count;
	size_t				go_count;
	size_t				selection_count;
	unsigned int		go_mat_update;
}						t_env;

/*
** setup.c				=> 5 functions (+ 1 to move)
*/
t_env					*init_scop(t_env *env, int argc, char **argv);

/*
** shader_init.c		=> 5 functions
*/
GLuint					init_program(t_shader *program, const char *path, \
							t_uniforms uf);

void		get_uniforms(t_shader *shdr, t_uniforms shader_u);

/*
** parser.c				=> 3 functions
*/
t_obj_parser_var		*init_opv(t_obj_parser_var *opv, char *name, \
							unsigned int mtl_offset);
void					parse_file(t_env *env, const char *path, \
							void (*ft_parsing)(t_env*, t_parser*, char*));

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

/*
** node_create.c		=> 4 functions
*/
t_go_node				*create_go_node(char *name, unsigned int mtl_id, \
							size_t ic);
t_mtl_node				*create_mtl_node(char *name);

/*
** node_add.c			=> 2 functions (+ 1 to move)
*/
t_go_node				*add_go_node(t_env *env, t_go_node *node);
t_mtl_node				*add_mtl_node(t_env *env, t_mtl_node *node);
//t_go_node				*clone_go_node(t_go_node *src);

/*
** update.c				=> 1 function
*/
unsigned int			mat_update(t_mat4x4 **mat, size_t count);

/*
** node_clean.c			=> 4 functions
*/
void					clean_go_node(t_go_node *node, unsigned int no_free);
void					clean_mtl_node(t_mtl_node *node, unsigned int no_free);
void					remove_go_node(t_go_node *list, unsigned int id, \
							unsigned int no_f, size_t *count);

/*
** cleanup.c			=> 4 functions
*/
void					*clean_scop(t_env *env, t_clean_flags f);

/*
** error.c				=> 5 functions
*/
void					log_error(const char *msg);
void					log_error_free(char *msg);
void					*log_error_null(const char *msg);
int						parser_error(const char *err, const char *fname, unsigned int fline);
GLuint					shader_error(const char *shader_name, char *log, GLenum shader_type); //shader_type is unused

// removal needed
void					display_quaternion(t_quaternion q, const char *msg);
void					display_mat4x4(t_mat4x4 mat, const char *msg);
void					display_vec3(const char *p, t_vec3 v);
void					display_gameobject(t_gameobject *obj);
void					display_go_list(t_go_node *list);
void					display_material(t_material *mtl);
void					display_mtl_list(t_mtl_node *list);
//

void					init_gl_objects(t_gameobject *go, char *b, size_t sf);
t_transform				init_transform(void);
t_transform         	init_transform_trs(t_vec3 t, t_quaternion r, t_vec3 s);
t_mat4x4				go_trs(t_transform tr);
t_light					init_light(t_env *env, t_vec3 color, float i, \
							float range);
							
t_gameobject			*get_gameobject(t_go_node *list, unsigned int id);

#endif
