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

int				ftpf_handle_float(t_par *p, va_list ap, t_buf *buf)
{
	(void)(p);
	(void)(buf);
	return 0;
}

int				no_conv(void)
{
	return 0;
}

static void		function_factory(void)
{
	g_functable['i'] = ftpf_handle_int;
	g_functable['d'] = ftpf_handle_int;
	g_functable['D'] = ftpf_handle_int;
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

static int		ftpf_majortom(const char **format, t_par *p,
	t_buf *buf, va_list ap)
{
	++*format;
	ftpf_get_format_flag(p, format);
	ftpf_get_width(p, format, ap);
	ftpf_get_precision(p, format, ap);
	ftpf_get_size_flag(p, format);
	ftpf_get_type(p, format);
	return (!g_functable[p->type]
		? no_conv()
		: g_functable[p->type](p, ap, buf));
}

int				ftpf_groundcontrol(const char *format, va_list ap)
{
	t_par	p;
	t_buf	buf;

	ft_memset(&buf, 0, sizeof(buf));
	function_factory();
	while (*format)
	{
		ft_memset(&p, 0, sizeof(p));
		if (*format != '%')
			format += ftpf_buffer_literal(format, &buf);
		else
			if (!(ftpf_majortom(&format, &p, &buf, ap)))
				return (-1);
	}
	write(1, buf.content, buf.cursor);
	return (buf.ret);
}
