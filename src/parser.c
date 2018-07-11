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

/*
** check perf if the array is declared as static global.
** But yea, it's an array of function pointers.
** It obviously uses the conversion specifiers as indexes, 
** to match them with the proper functions.
*/

static t_fct	function_factory(int i)
{
	t_fct	functable[127];

	ft_memset(functable, 0, sizeof(functable));
	functable['d'] = ftpf_handle_int;
	functable['D'] = ftpf_handle_int;
	functable['i'] = ftpf_handle_int;
	functable['u'] = ftpf_handle_int;
	functable['U'] = ftpf_handle_int;
	functable['o'] = ftpf_handle_int;
	functable['O'] = ftpf_handle_int;
	functable['x'] = ftpf_handle_int;
	functable['X'] = ftpf_handle_int;
	functable['b'] = ftpf_handle_int;
	functable['B'] = ftpf_handle_int;
	functable['f'] = ftpf_handle_float;
	functable['F'] = ftpf_handle_float;
	functable['c'] = ftpf_handle_char;
	functable['C'] = ftpf_handle_wchar;
	functable['s'] = ftpf_handle_str;
	functable['S'] = ftpf_handle_wstr;
	functable['p'] = ftpf_handle_ptr;
	functable['P'] = ftpf_handle_ptr;
	functable['n'] = ftpf_handle_n;
	return (functable[i]);
}

/*
** if no conversion specifier is found, print the char that's where 
** the specifier should be. With width and precision applied to it.
*/

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

/*
** calls the options check function, then cleans up the mess in the flags,
** by disabling those that won't be used, espcially for width handling.
** It looks quite ugly, but it makes everything else much clearer.
*/

static int		ftpf_majortom(const char **format, t_par *p,
	t_buf *buf, va_list ap)
{
	int	i;

	++*format;
	if (**format == '{' && (i = ft_findchar(*format, '}')) >= 0)
		return (ftpf_handle_display(format, buf, i));
	ftpf_get_format_flag(p, format);
	ftpf_get_width(p, format, ap);
	ftpf_get_precision(p, format, ap);
	ftpf_get_size_flag(p, format);
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
	return (!function_factory(p->type)
		? no_conv(p, buf, format) : function_factory(p->type)(p, ap, buf));
}

int				ftpf_groundcontrol(const char *format, va_list ap)
{
	t_par	p;
	t_buf	buf;

	ft_memset(&buf, 0, sizeof(buf));
	buf.str = NULL;
	while (*format)
	{
		ft_memset(&p, 0, sizeof(p));
		if (*format != '%')
			format += ftpf_buffer_literal(format, &buf);
		else if (!(ftpf_majortom(&format, &p, &buf, ap)))
		{
			write(1, buf.content, buf.cursor);
			return (-1);
		}
	}
	buf.ret += write(1, buf.content, buf.cursor);
	return (buf.ret);
}
