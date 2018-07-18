/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_display.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/18 17:50:56 by pfragnou          #+#    #+#             */
/*   Updated: 2018/07/18 17:51:00 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
** Ain't that pretty ?
*/

static char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	if (!*needle)
		return ((char*)haystack);
	if (!*haystack || len == 0)
		return (NULL);
	return ((*needle == *haystack
		&& ft_strnstr(haystack + 1, needle + 1, len - 1) == haystack + 1)
		? (char*)haystack : ft_strnstr(haystack + 1, needle, len - 1));
}

/*
** Use enums for color and style
*/

static int	get_color(const char *format, int len)
{
	int		i;
	char	table[8][7] = {"black",
							"red",
							"green",
							"yellow",
							"blue",
							"magenta",
							"cyan",
							"white"};

	i = 0;
	while (i < 8)
	{
		if (ft_strnstr(format, table[i], len))
			return (i);
		++i;
	}
	return (7);
}

/*
** Only takes the first parameter encountered, which doesn't matter since
** only one can be added in a single ANSI SGR code.
** Also, it fucking sticks until an eoc flag. Gotta find a way around that.
*/

static int	get_style(const char *format, int len)
{
	int		i;
	char	table[4][9] = {"bold",
							"faint",
							"italic",
							"underline"};

	i = 0;
	while (i < 4)
	{
		if (ft_strnstr(format, table[i], len))
			return (i + 1);
		++i;
	}
	return (0);
}

/*
** Use strnstr to find color and style flags, or eoc. Nothing weird here.
** \e[s;3cm] : s for style, c for color. simple place holders.
** 3 is for regular foreground colors (30 to 37),
** m is the ANSI graphic rendition control sequence identifier.
** This is a very basic implementation,
** i'd like to make it a bit more functional, by adding the possibility of
** selecting multiple styles at once, and removing them independently.
** The only way i found to do it so far is to print the codes everytime.
** Working on an smart implementation for that.
*/

int			ftpf_handle_display(const char **format, t_buf *buf, int len)
{
	char	disp_code[7];

	++*format;
	if (ft_strnstr(*format, "eoc", len))
		ftpf_buffer_copy("\e[m", buf, 3);
	else
	{
		ft_memcpy(disp_code, "\e[s;3cm", 7);
		disp_code[2] = get_style(*format, len) + '0';
		disp_code[5] = get_color(*format, len) + '0';
		ftpf_buffer_copy(disp_code, buf, 7);
	}
	*format += len;
	return (1);
}
