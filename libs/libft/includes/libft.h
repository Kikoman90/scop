/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 13:21:24 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/12 19:07:44 by fsidler          ###   ########.fr       */
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

# define UC unsigned char

typedef struct		s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}					t_list;

typedef struct		s_info
{
	double			a;
	int				e;
	int				c;
	int				sign;
	int				i;
	int				count;
}					t_info;

int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isspace(int c);
int					ft_isblank(int c);

int					ft_tolower(int c);
int					ft_toupper(int c);

int					ft_atoi(const char *str);
int					ft_atoi_f(char *str);
float				ft_atof(const char *s);
float				ft_atof_f(char *s);

int					ft_iclamp(int value, const int min, const int max);
float				ft_fclamp(float value, const float min, const float max);

void				ft_bzero(void *s, size_t n);

void				*ft_memccpy(void *dest, const void *src, int c, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
void				*ft_memmove(void *dst, const void *src, size_t len);
void				*ft_memset(void *s, int c, size_t n);
void				ft_fmemset(float *s, float v, size_t n);

int					ft_memcmp(const void *s1, const void *s2, size_t n);

int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);

char				*ft_strcat(char *dest, const char *src);
char				*ft_strncat(char *dest, const char *src, size_t n);
char				*ft_strchr(const char *str, int c, int inc);
char				*ft_strnchr(const char *str, int c, int n, int inc);
char				*ft_strrchr(const char *str, int c);
char				*ft_strcpy(char *dest, const char *src);
char				*ft_strncpy(char *dest, const char *src, size_t n);
char				*ft_strdup(const char *str);
char				*ft_strstr(const char *tot, const char *fnd);
char				*ft_strnstr(const char *total, const char *find, size_t n);

size_t				ft_strlen(const char *str);
size_t				ft_strlcat(char *dest, const char *src, size_t n);

void				*ft_memalloc(size_t size);
void				ft_memdel(void **ap);

void				ft_strdel(char **as);
void				ft_strclr(char *s);
void				ft_striter(char *s, void (*f)(char *));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));

void				ft_putchar(char c);
void				ft_putstr(char const *s);
void				ft_putendl(char const *s);
void				ft_putnbr(int n);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char const *s, int fd);
void				ft_putendl_fd(char const *s, int fd);
void				ft_putnbr_fd(int n, int fd);

char				*ft_strnew(size_t size);
char				*ft_strmap(char const *s, char (*f)(char));
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char				*ft_strsub(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strjoin_bf(char *s1, char *s2);
char				*ft_strjoin_lf(char *s1, char const *s2);
char				*ft_strjoin_rf(char const *s1, char *s2);
char				*ft_strtrim(char const *s);
char				*ft_strword(char const *s, unsigned int *offset);

unsigned int		ft_wordoffset(char const *s, unsigned int offset);
unsigned int		skip_line(char *data, unsigned int seed);

char				*ft_itoa(int n);

char				**ft_strsplit(char const *s, char c);

int					ft_strequ(char const *s1, char const *s2);
int					ft_strnequ(char const *s1, char const *s2, size_t n);

int					ft_abs(int n);
int					ft_signbit(float n);

unsigned char		ft_bitrev(unsigned char c);

void				ft_lstdelone(t_list **alst, void (*del)(void *, size_t));
void				ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void				ft_lstadd(t_list **alst, t_list *new);
void				ft_lstiter(t_list *lst, void (*f)(t_list *elem));

t_list				*ft_lstnew(void const *content, size_t content_size);
t_list				*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));

double				ft_round(double x);
double				ft_fdim(double n1, double n2);
double				ft_copysign(double n1, double n2);

void				*ft_file_map(const char *path, size_t *data_size);
void				ft_file_unmap(void *data, size_t fsize, char *fpath);

unsigned int		log_error(const char *msg);
unsigned int		log_error_free(char *msg);
void				*log_error_null(const char *msg);

#endif
