/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 15:14:06 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/20 20:39:57 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include <time.h>

# include "parser.h"
# include "texture.h"

# include <SDL2/SDL.h>

# define WIN_W 1600
# define WIN_H 900

# define SDL_INIT_ERROR "failed to initialize sdl"
# define WIN_CREATE_ERROR "failed to create window"
# define SHADER_INIT_ERROR "shader initialization failed"
# define FRAMEBUFFER_INCOMPLETE_ERROR "framebuffer imcomplete"

# define GO_NAME "gameobject_"
# define MTL_NAME "material_"

# define GO_ID_OFFSET 0x14
# define MTL_ID_OFFSET 0x1

# define BILLBOARD_SHADER 0
# define DEFAULT_SHADER 1
# define PICK_SHADER 2
# define PRIMITIVE_SHADER 3
# define SKYBOX_SHADER 4
# define STANDARD_SHADER 5

# define SCOP_CIRCLE 0
# define SCOP_CONE 1
# define SCOP_CUBE 2
# define SCOP_LINE 3
# define SCOP_PLANE 4
# define SCOP_SPHERE 5

typedef enum			e_handlemode
{
	SCOP_TRANSLATE = 1 << 1,
	SCOP_ROTATE = 1 << 2,
	SCOP_SCALE = 1 << 3
}						t_handlemode;

typedef struct			s_geometry
{
	char				*name;
	GLuint				vao;
	GLuint				vbo;
	size_t				count;
}						t_geometry;

typedef struct			s_shader
{
	const char			*name;
	GLuint				vtx_s;
	GLuint				frg_s;
	GLuint				prog;
	GLint				u_loc[13];
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
	t_vec4				color;
}						t_vtx_attrib;

typedef struct			s_gameobject
{
	char				*name;
	t_transform			transform;
	t_vtx_attrib		*vtx_attrib;
	size_t				vtx_count;
	GLuint				vao;
	GLuint				vbo;
	unsigned int		mtl_id;
	t_vec3				pick_clr;
	float				bounds[6];
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

typedef struct			s_tr_node
{
	unsigned int		id;
	t_transform			*transform;
	struct s_tr_node	*next;
}						t_tr_node;

typedef struct			s_tr_list
{
	t_tr_node			*head;
	size_t				count;
}						t_tr_list;

typedef struct			s_light
{
	t_transform			transform;
	t_vec3				color[4];
	float				intensity;
	float				range;
	t_vec3				pick_clr;
	unsigned int		id;
	GLuint				texture_id;
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
	t_mat4x4			v;
	t_mat4x4			p;
	unsigned int		update_mat[3];
}						t_mvp_mat;

typedef struct			s_selection
{
	t_tr_list			list;
	t_transform			transform;
	t_handlemode		mode;
	int					localspace;
	int					active;
	int					type;
	t_mat4x4			rot[4];
	t_vec4				clr[4];
	t_vec2				motion_axis;
}						t_selection;

typedef struct			s_inputstate
{
	unsigned int		cur_sky;
	unsigned int		cur_tex;
	float				pan_speed;
	float				rot_speed;
	float				zoom_speed;
	float				fade;
	float				fade_coef;
	int					face_rgb;
	int					auto_rotate;
}						t_inputstate;

typedef struct			s_env
{
	t_win				win_env;
	t_gl_buffers		buffers;
	t_shader			shaders[6];
	t_geometry			primitives[6];
	GLuint				textures[7];
	GLuint				skyboxes[4];
	t_light				light;
	t_camera			camera;
	t_mvp_mat			matrices;
	t_go_list			gameobjects;
	t_mtl_list			materials;
	t_selection			selection;
	t_inputstate		input;
	double				delta_time;
	int					loop;
}						t_env;

/*
** setup.c				=> 5 functions
*/
unsigned int			init_scop(t_env *env, int argc, char **argv);

/*
** shader.c				=> 5 functions
*/
unsigned int			init_shaders(unsigned int nb, const char *path, \
								t_shader *shader);

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
** attrib_parse.c		=> 5 functions
*/
unsigned int			parse_face(t_gameobject *go, t_obj_parser_var *opv, \
							t_parser *parser, unsigned int seed);

/*
** obj_parse.c			=> 5 functions
*/
void					parse_wavefrontobj(t_go_list *gameobjects, \
							t_mtl_list *materials, t_parser *parser, \
							char *word);

/*
** obj_get.c			=> 5 functions
*/
int						get_shader_idx(t_shader (*tab)[6], \
							const char *name);
void					get_model_matrices(t_go_node *go_list, t_mat4x4 *m);
unsigned int			get_mtl_id(t_mtl_node *list, char *mtl_name, \
							unsigned int mtl_offset);
t_material				*get_mtl(t_mtl_node *list, unsigned int id);
t_gameobject			*get_gameobject(t_go_node *list, unsigned int id);

/*
** node_create.c		=> 5 functions
*/
t_mtl_node				*create_mtl_node(char *name);
t_go_node				*create_go_node(char *name, unsigned int mtl_id, \
							size_t ic);
t_tr_node       		*create_tr_node(unsigned int id, \
							t_transform *transform);

/*
** node_add.c			=> 5 functions
*/
t_vec3					generate_pick_clr(unsigned int id);
void					add_mtl_node(t_mtl_list *list, t_mtl_node *node);
void					add_go_node(t_go_list *list, t_go_node *node);

/*
** node_clean.c			=> 5 functions
*/
void					clean_mtl_node(t_mtl_node *node, int free_mtl);
void					remove_mtl_node(t_mtl_list *list, unsigned int id, \
							int free_mtl);
void					clean_go_node(t_go_node *node, int free_go);
void					remove_go_node(t_go_list *list, unsigned int id, \
							int free_go);
unsigned int			remove_tr_node(t_tr_list *list, unsigned int id);

/*
** buffers_bind.c		=> 2 functions
*/
unsigned int			generate_framebuffers(t_gl_buffers *buffers, \
							unsigned int win_w, unsigned int win_h);

/*
** get_uniforms.c		=> 4 functions
*/
void					get_uniforms(t_shader *shader);

/*
** set_uniforms.c		=> 5 functions
*/
void					set_uniforms(t_env *env, t_shader *shader, \
							t_go_node *node);

/*
** uniforms.c			=> 4 functions
*/
void					set_uniforms(t_env *env, t_shader *shader, \
							t_go_node *node);
void					get_uniforms(t_shader *shader);

/*
** selection.c			=> 3 functions
*/
void        			selection_transform(t_selection *selection);
void					picking_check(t_env *env, int x, int y, Uint8 lshift);

/*
** events_handle.c		=> 5 functions
*/
void					handle_events_and_input(t_env *env);

/*
** update.c				=> 3 function
*/
void					rotate_gameobjects(t_go_node *list, double delta);
void					update_view(t_env *env, t_vec3 d, t_handlemode mode);
void					update_matrices(t_env *env, int update);

/*
** draw_handles.c		=> 5 functions
*/
void					draw_translate_handles(t_env *env, t_shader *shd);
void					draw_rotate_handles(t_env *env, t_shader *shd);
void					draw_scale_handles(t_env *env, t_shader *shd);

/*
** draw.c				=> 5 functions
*/
void					draw(t_env *env);

/*
** cleanup.c			=> 4 functions
*/
void					clear_mtl_list(t_mtl_list *list, int free_mtl);
void					clear_go_list(t_go_list *list, int free_go);
void					clear_tr_list(t_tr_list *list);
unsigned int			clean_scop(t_env *env);

// display.c (remove)
void					display_vec3(const char *p, t_vec3 v);
void					display_mat4x4(t_mat4x4 mat, const char *msg);
void					display_quaternion(t_quaternion q, const char *msg);
void					display_material(t_material *mtl);
void					display_mtl_list(t_mtl_node *list);
void					display_gameobject(t_gameobject *obj);
void					display_go_list(t_go_node *list);
//

#endif
