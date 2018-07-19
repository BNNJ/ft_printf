/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_display.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/18 18:49:00 by pfragnou          #+#    #+#             */
/*   Updated: 2018/07/18 18:49:01 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	g_displaytable[18][10];

static void	ftpf_init_table(void)
{
	ft_memcpy(g_displaytable[0], "reset", 5);
	ft_memcpy(g_displaytable[1], "bold", 4);
	ft_memcpy(g_displaytable[3], "italic", 6);
	ft_memcpy(g_displaytable[4], "underline", 9);
	ft_memcpy(g_displaytable[5], "/bold", 5);
	ft_memcpy(g_displaytable[6], "/italic", 7);
	ft_memcpy(g_displaytable[7], "/underline", 10);
	ft_memcpy(g_displaytable[8], "black", 5);
	ft_memcpy(g_displaytable[9], "red", 3);
	ft_memcpy(g_displaytable[10], "green", 5);
	ft_memcpy(g_displaytable[11], "yellow", 6);
	ft_memcpy(g_displaytable[12], "blue", 4);
	ft_memcpy(g_displaytable[13], "magenta", 7);
	ft_memcpy(g_displaytable[14], "cyan", 4);
	ft_memcpy(g_displaytable[15], "white", 5);
	ft_memcpy(g_displaytable[16], "rgb", 3);
	ft_memcpy(g_displaytable[17], "eoc", 3);
}

static int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (--n && *s1 && *s2 && *s1 == *s2)
	{
		++s1;
		++s2;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

static int	ftpf_get_option(const char **format)
{
	int	i;
	int	len;

	i = 0;
	len = ft_findchar(*format, ':');
	len = len < ft_findchar(*format, '}') && len > 0
		? len : ft_findchar(*format, '}');
	while (i <= 18)
	{
		if (!ft_strncmp(g_displaytable[i], *format, len))
		{
			*format += len;
			if (i >= 0 && i <= 4)
				return (i);
			else if (i >= 5 && i <= 7)
				return (i + 17);
			else if (i >= 8 && i <= 17)
				return (i + 22);
		}
		++i;
	}
	return (-1);
}

static void	ftpf_buffer_display(t_buf *buf, int option)
{
	if (option <= 9)
		ftpf_buffer_fill(buf, option + '0', 1);
	else
	{
		ftpf_buffer_fill(buf, option / 10 + '0', 1);
		ftpf_buffer_fill(buf, option % 10 + '0', 1);
	}
}

int			ftpf_handle_display(const char **format, t_buf *buf, int len)
{
	int		read;
	int		option;

	read = 0;
	++*format;
	ftpf_init_table();
	ftpf_buffer_copy("\e[", buf, 2);
	while (**format != '}')
	{
		if ((option = ftpf_get_option(format)) < 0)
			break ;
		ftpf_buffer_display(buf, option);
		if (**format == ':')
		{
			ftpf_buffer_fill(buf, ';', 1);
			++*format;
		}
	}
	++*format;
	ftpf_buffer_fill(buf, 'm', 1);
	return (1);
}
