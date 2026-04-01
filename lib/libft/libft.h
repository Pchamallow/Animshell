/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:50:27 by pswirgie          #+#    #+#             */
/*   Updated: 2026/02/15 17:25:31 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <ctype.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <stdint.h>

typedef struct s_list
{
	int				content;
	int				index;
	struct s_list	*next;
}					t_list;

// Part 1 - Libc functions
long	ft_atol(char *str);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
int		ft_isalpha(char c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isprint(int c);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strdup(const char *src);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

// Part 2 - Additional functions
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
int		ft_putchar_fd(char c, int fd);
int		ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
int		ft_putnbr_fd(int n, int fd);
int		ft_putunbr_fd(long int n, int fd);

// Bonus Part
t_list	*ft_lstnew(int content);
void	ft_lstadd_front(t_list **lst, t_list *newer);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *newer);
// void	ft_lstdelone(t_list *lst, void (*del)(int));
void	ft_lstclear(t_list **head);
// void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

// PRINTF
int		ft_printf_fd(int fd, const char *format, ...);
int		ft_putnbr_st_fd(size_t n, size_t baseformat, char *base, int fd);
int		ft_putnbr_ui_fd(unsigned int n,
			unsigned int baseformat, char *base, int fd);
int		ft_putpointer(int n, int fd);
int		ft_printhexalow(va_list args, int fd);
int		ft_printhexaup(va_list args, int fd);
int		ft_printchar(va_list args, int fd);
int		ft_printdecimal(va_list args, int fd);
int		ft_printstr(va_list args, int fd);
int		ft_printpercent(va_list args, int fd);
int		ft_printnbr(va_list args, int fd);
int		ft_printunbr(va_list args, int fd);
int		ft_printpointer(va_list args, int fd);
int		ft_line(char format, int fd);
int		ft_percenterror(const char *format, va_list args);

#endif
