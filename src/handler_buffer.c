/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_buffer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/28 17:44:15 by pfragnou          #+#    #+#             */
/*   Updated: 2018/06/28 17:44:17 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ftpf_buffer_literal(const char *str, t_buf *buf)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '%')
	{
		if (buf->cursor == BUFFSIZE)
		{
			buf->ret += write(1, buf->content, BUFFSIZE);
			buf->cursor = 0;
		}
		buf->content[buf->cursor] = str[i];
		++buf->cursor;
		++i;
	}
	return (i);
}

void	ftpf_buffer_copy(const char *str, t_buf *buf, int precision)
{
	int	i;

	i = 0;
	while (i < precision && str[i])
	{
		if (buf->cursor == BUFFSIZE)
		{
			buf->ret += write(1, buf->content, BUFFSIZE);
			buf->cursor = 0;
		}
		buf->content[buf->cursor] = str[i];
		++buf->cursor;
		++i;
	}
}

void	ftpf_buffer_fill(t_buf *buf, char c, size_t size)
{
	while (size > 0)
	{
		if (buf->cursor == BUFFSIZE)
		{
			write(1, buf->content, BUFFSIZE);
			buf->cursor = 0;
		}
		buf->content[buf->cursor] = c;
		++buf->cursor;
		++buf->ret;
		--size;
	}
}
