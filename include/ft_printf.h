/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 15:24:04 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/03/07 15:23:44 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdint.h>
# include <limits.h>
# include <unistd.h>

char	*ft_strchr(const char *s, int c);
int		ft_putchar_fd(char c, int fd);
int		ft_putstr_fd(char *s, int fd);
int		ft_putdec_fd(int nbr, char *base, int fd);
int		ft_puthex_fd(int nbr, char *base, int fd);
int		ft_putuint_fd(unsigned int nbr, int fd);
int		ft_putaddr_fd(void *ptr, int fd);
int		ft_printf(const char *format, ...);

#endif
