/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 15:14:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/09 20:33:34 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include <time.h>

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

# define GO_NAME "gameobject_"
# define MTL_NAME "material_"

typedef struct			s_shader
{
	const char			*name;
	GLuint				vtx_s;
	GLuint				frg_s;
	GLuint				prog;
	GLint				u_loc[12];
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

typedef struct			s_mtl_node
{
	unsigned int		id;
	t_material			*mtl;
	struct s_mtl_node	*next;
}						t_mtl_node;

typedef struct			s_mtl_list
{
	t_mtl_node			*head;
	size_t				count;
}						t_mtl_list;

typedef struct			s_vtx_attrib
{
	t_vec2				uv;
	t_vec3				position;
	t_vec3				normal;
	t_vec3				color;
}						t_vtx_attrib;

typedef struct			s_gameobject
{
	char				*name;
	t_transform			transform;
	t_vtx_attrib		*vtx_attrib;
	size_t				vtx_count;
	unsigned int		mtl_id;
	t_vec3				pick_clr;
	float				bounds[6];
	GLuint				vao;
	GLuint				vbo;
}						t_gameobject;

typedef struct			s_go_node
{
	unsigned int		id;
	t_gameobject		*go;
	struct s_go_node	*next;
}						t_go_node;

typedef struct			s_go_list
{
	t_go_node			*head;
	size_t				count;
}						t_go_list;

typedef struct			s_light
{
	t_transform			transform;
	t_vec3				color;
	float				intensity;
	float				range;
}						t_light;

typedef struct			s_camera
{
	t_transform			transform;
	float				fov;
	float				znear;
	float				zfar;
}						t_camera;

typedef struct			s_win
{
	SDL_Window			*window;
	SDL_GLContext		gl_context;
	unsigned int		win_w;
	unsigned int		win_h;
}						t_win;

typedef struct			s_gl_buffers
{
	GLuint				rbo[4];
	GLuint				ms_fbo;
	GLuint				pick_fbo;
}						t_gl_buffers;

typedef struct			s_mvp_mat
{
	t_mat4x4			*model;
	t_mat4x4			view;
	t_mat4x4			projection;
	t_mat4x4			vp;
	unsigned int		update_mat[3];
}						t_mvp_mat;

typedef struct			s_input
{
	float				pan_speed;
	float				zoom_speed;
	float				orbit_speed;
	float				go_move_speed;
	float				go_rot_speed;
	float				go_scale_speed;
	int					world_space;
}						t_input;

typedef struct			s_env
{
	int					loop;
	t_win				win_env;
	t_gl_buffers		buffers;
	t_shader			shaders[3];
	t_camera			camera;
	t_light				light;
	t_mvp_mat			matrices;
	t_mtl_list			materials;
	t_go_list			gameobjects;
	t_go_list			selection;
	t_input				input;
}						t_env;

/*
** setup.c				=> 5 functions
*/
unsigned int			init_scop(t_env *env, int argc, char **argv);

/*
** shader.c				=> 4 functions
*/
GLuint					init_program(t_shader *program, const char *path, \
							unsigned int idx);
/*
** parser.c				=> 5 functions
*/
void					init_opv(t_obj_parser_var *opv, char *name, \
							unsigned int mtl_offset);
unsigned int			parser_error(const char *error, const char *fname, \
							unsigned int fline);
void					parse_file(t_go_list *gameobjects, \
							t_mtl_list *materials, const char *path, \
							void (*parse)(t_go_list*, t_mtl_list*, \
							t_parser*, char*));

/*
** mtl_parse.c			=> 3 functions
*/
void					parse_wavefrontmtl(t_go_list *gameobjects, \
							t_mtl_list *materials, t_parser *parser, \
							char *word);

/*
** attrib_parse.c		=> 4 functions
*/
unsigned int			parse_faces(t_gameobject *go, t_obj_parser_var *opv, \
							t_parser *parser, char *w);

/*
** obj_parse.c			=> 5 functions
*/
void					parse_wavefrontobj(t_go_list *gameobjects, \
							t_mtl_list *materials, t_parser *parser, \
							char *word);

/*
** obj_get.c			=> 4 functions
*/
void					get_model_matrices(t_go_node *go_list, t_mat4x4 *m);
unsigned int			get_mtl_id(t_mtl_node *list, char *mtl_name, \
							unsigned int mtl_offset);
t_material				*get_mtl(t_mtl_node *list, unsigned int id);
t_gameobject			*get_gameobject(t_go_node *list, unsigned int id);

/*
** node_create.c		=> 4 functions
*/
t_mtl_node				*create_mtl_node(char *name);
t_go_node				*create_go_node(char *name, unsigned int mtl_id, \
							size_t ic);

/*
** node_add.c			=> 3 functions
*/
void					add_mtl_node(t_mtl_list *list, t_mtl_node *node);
void					add_go_node(t_go_list *list, t_go_node *node);

/*
** node_clean.c			=> 4 functions
*/
void					clean_mtl_node(t_mtl_node *node, int free_mtl);
void					remove_mtl_node(t_mtl_list *list, unsigned int id, \
							int free_mtl);
void					clean_go_node(t_go_node *node, int free_go);
void					remove_go_node(t_go_list *list, unsigned int id, \
							int free_go);

/*
** buffers_bind.c		=> 2 functions
*/
unsigned int			generate_framebuffers(t_gl_buffers *buffers, \
							unsigned int win_w, unsigned int win_h);

/*
** uniforms.c			=> 4 functions
*/
void					set_uniforms(t_env *env, t_go_node *node, \
							unsigned int idx);
void					get_uniforms(t_shader *shdr, unsigned int idx);

/*
** events_handle.c		=> 4 functions
*/
void					handle_events_and_input(t_env *env, double delta_time);

/*
** update.c				=> 2 function
*/
void					rotate_gameobjects(t_go_node *list, double delta);
void					update_matrices(t_env *env, int update);
/*void					update_view(t_camera *camera, \
							SDL_MouseMotionEvent motion, double delta_time, \
							float speed);*/

/*
** draw.c				=> 3 functions
*/
void					draw(t_env *env);

/*
** cleanup.c			=> 3 functions
*/
void					clear_mtl_list(t_mtl_list *list, int free_mtl);
void					clear_go_list(t_go_list *list, int free_go);
unsigned int			clean_scop(t_env *env);

// removal needed
void					display_vec3(const char *p, t_vec3 v);
void					display_mat4x4(t_mat4x4 mat, const char *msg);
void					display_quaternion(t_quaternion q, const char *msg);
void					display_material(t_material *mtl);
void					display_mtl_list(t_mtl_node *list);
void					display_gameobject(t_gameobject *obj);
void					display_go_list(t_go_node *list);
//

#endif
