/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 13:21:24 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/29 14:40:38 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include <sys/types.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <fcntl.h>

# include <errno.h>
# include <dirent.h>

# define UC unsigned char

# define MALLOC_ERROR "memory allocation failure"
# define MISSING_FILE_ERROR "missing file at path "

typedef struct		s_info
{
	double			a;
	int				e;
	int				c;
	int				sign;
	int				i;
	int				count;
}					t_info;

void				ft_bzero(void *s, size_t n);

int					ft_isdigit(int c);
int					ft_isblank(int c);

int					ft_atoi(const char *str);
float				ft_atof_f(char *s);

int					ft_iclamp(int value, const int min, const int max);
float				ft_fclamp(float value, const float min, const float max);

int					ft_strcmp(const char *s1, const char *s2);

char				*ft_strcat(char *dest, const char *src);
char				*ft_strchr(const char *str, int c, int inc);
char				*ft_strnchr(const char *str, int c, int n, int inc);
char				*ft_strrchr(const char *str, int c);
char				*ft_strcpy(char *dest, const char *src);
char				*ft_strdup(const char *str);

size_t				ft_strlen(const char *str);

void				ft_putchar(char c);
void				ft_putstr(char const *s);
void				ft_putendl(char const *s);

char				*ft_strnew(size_t size);
char				*ft_strsub(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strjoin_bf(char *s1, char *s2);
char				*ft_strjoin_lf(char *s1, char const *s2);
char				*ft_strjoin_rf(char const *s1, char *s2);
char				*ft_strword(char const *s, unsigned int *offset);

unsigned int		ft_wordoffset(char const *s, unsigned int offset);
unsigned int		skip_line(char *data, unsigned int seed);

char				*ft_itoa(int n);

void				*ft_file_map(const char *path, size_t *data_size);
void				ft_file_unmap(void *data, size_t fsize, char *fpath);

void				ft_free_file_names(char **names, unsigned int nb);
char				**ft_get_file_names(const char *dir_path, unsigned int nb, \
						int sort);

unsigned int		log_error(const char *msg);
unsigned int		log_error_free(char *msg);
void				*log_error_null(const char *msg);
void				*log_error_null_free(char *msg);

#endif
