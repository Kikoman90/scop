/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 10:38:04 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/14 15:08:16 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static int	init_parser(t_parser *parser, const char *path, int *fd)
{
	struct stat	s;

	if ((*fd = open(path, O_RDWR)) == -1)
	{
		log_error_free(ft_strjoin("(open) ", strerror(errno)));
		return (-1);
	}
	if (fstat(*fd, &s) == -1)
	{
		close(*fd);
		log_error_free(ft_strjoin("(fstat) ", strerror(errno)));
		return (-1);
	}
	parser->fsize = (size_t)s.st_size;
	parser->fname = ft_strrchr(path, '/') + 1;
	parser->fpath = ft_strsub(path, 0, ft_strlen(path) - ft_strlen(parser->fname));
	parser->fline = 1;
	parser->data = NULL;
	return (0);
}

void		parse_file(t_env *env, const char *path, void (*ft_parsing)(t_env*, t_parser*, int))
{
	t_parser	*parser;
	int			fd;

	if (!(parser = (t_parser*)malloc(sizeof(t_parser))))
		log_error(MALLOC_ERROR);
	if (init_parser(parser, path, &fd) != -1)
	{
		if ((parser->data = (char *)mmap(NULL, parser->fsize, \
			PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			log_error_free(ft_strjoin("(mmap) ", strerror(errno)));
		else
		{
			parser->data[parser->fsize] = '\0';
			ft_parsing(env, parser, 0);
			if (munmap(parser->data, parser->fsize) == -1)
				log_error_free(ft_strjoin("(munmap) ", strerror(errno)));
			parser->data = NULL;
		}
		close(fd);
		free(parser->fpath);
	}
	free(parser);
}

static void	*init_sdl_gl(t_env *env)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		return (log_error_null(SDL_INIT_ERROR));
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, \
		SDL_GL_CONTEXT_PROFILE_CORE);
	if (!(env->window = SDL_CreateWindow("SCOP", SDL_WINDOWPOS_CENTERED, \
		SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_RESIZABLE \
		| SDL_WINDOW_OPENGL)))
		return (log_error_null(WIN_CREATE_ERROR));
	if (!(env->gl_context = SDL_GL_CreateContext(env->window)))
		return (log_error_null(SDL_GetError()));
	glClearColor(0.19, 0.27, 0.41, 1);
	glViewport(0, 0, WIN_W, WIN_H);
	return ((void*)1);
}

t_env		*init_scop(t_env *env, int argc, char **argv)
{
	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (log_error_null(MALLOC_ERROR));
	if (!init_sdl_gl(env))
		return (clean_scop(env, CLEAN_SDL));
	//init_cam, init library, init serializer
	env->go_list = NULL;
	env->mtl_list = NULL;
	env->go_count = 0;
	env->mtl_count = 0;
	while (argc-- > 1)
		parse_file(env, argv[argc], parse_wavefrontobj);
	env->loop = 1;
	return (env);
}
