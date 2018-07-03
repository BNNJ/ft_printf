/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 12:47:49 by pfragnou          #+#    #+#             */
/*   Updated: 2018/06/27 12:47:51 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static t_fct	g_functable[127] = {NULL};

static void		function_factory(void)
{
	g_functable['d'] = ftpf_handle_int;
	g_functable['D'] = ftpf_handle_int;
	g_functable['i'] = ftpf_handle_int;
	g_functable['u'] = ftpf_handle_int;
	g_functable['U'] = ftpf_handle_int;
	g_functable['o'] = ftpf_handle_int;
	g_functable['O'] = ftpf_handle_int;
	g_functable['x'] = ftpf_handle_int;
	g_functable['X'] = ftpf_handle_int;
	g_functable['b'] = ftpf_handle_int;
	g_functable['B'] = ftpf_handle_int;
	g_functable['f'] = ftpf_handle_float;
	g_functable['F'] = ftpf_handle_float;
	g_functable['c'] = ftpf_handle_char;
	g_functable['C'] = ftpf_handle_wchar;
	g_functable['s'] = ftpf_handle_str;
	g_functable['S'] = ftpf_handle_wstr;
	g_functable['p'] = ftpf_handle_ptr;
	g_functable['P'] = ftpf_handle_ptr;
}

int				ftpf_handle_error(t_buf *buf, size_t size)
{
	buf->cursor = buf->cursor > size ? buf->cursor - size : 0;
	return (0);
}

static int		no_conv(t_par *p, t_buf *buf, const char **format)
{
	if (p->type != 0)
	{
		if (p->flags & F_BIN)
			return (ftpf_handle_bin(&p->type, p, buf));
		if ((p->flags & F_WIDTH || p->flags & F_ZERO) && p->width > 1)
			ftpf_buffer_fill(buf, p->flags & F_ZERO ? '0' : ' ', p->width - 1);
		ftpf_buffer_fill(buf, p->type, 1);
		if (p->flags & F_MINUS)
			ftpf_buffer_fill(buf, ' ', p->width - 1);
	}
	else
		--*format;
	return (1);
}

static int		ftpf_majortom(const char **format, t_par *p,
	t_buf *buf, va_list ap)
{
	++*format;
	ftpf_get_format_flag(p, format);
	ftpf_get_width(p, format, ap);
	ftpf_get_format_flag(p, format);
	ftpf_get_precision(p, format, ap);
	ftpf_get_format_flag(p, format);
	ftpf_get_size_flag(p, format);
	ftpf_get_format_flag(p, format);
	ftpf_get_type(p, format);
	if (p->type == 'o' || p->type == 'O' || p->type == 'x' || p->type == 'X')
	{
		p->flags &= ~F_PLUS;
		p->flags &= ~F_SPACE;
	}
	if ((p->flags & F_PRECI && (ft_findchar("dDioOuUxXpPbB", p->type) >= 0))
		|| !(p->flags & F_WIDTH) || p->flags & F_MINUS)
		p->flags &= ~F_ZERO;
	if (!(p->flags & F_WIDTH))
		p->flags &= ~F_MINUS;
	if (p->flags & F_ZERO || p->flags & F_MINUS)
		p->flags &= ~F_WIDTH;
	return (!g_functable[p->type]
		? no_conv(p, buf, format)
		: g_functable[p->type](p, ap, buf));
}

int				ftpf_groundcontrol(const char *format, va_list ap)
{
	t_par	p;
	t_buf	buf;

	ft_memset(&buf, 0, sizeof(buf));
	function_factory();
	buf.str = NULL;
	while (*format)
	{
		ft_memset(&p, 0, sizeof(p));
		if (*format != '%')
			format += ftpf_buffer_literal2(format, &buf);
		else if (!(ftpf_majortom(&format, &p, &buf, ap)))
		{
			write(1, buf.content, buf.cursor);
			return (-1);
		}
	}
	buf.ret += write(1, buf.content, buf.cursor);
	return (buf.ret);
}
