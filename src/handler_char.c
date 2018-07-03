/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/28 17:29:07 by pfragnou          #+#    #+#             */
/*   Updated: 2018/06/28 17:29:09 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			ftpf_convert_wchar(wchar_t c, char *str)
{
	if (c < -1 || (c >= 0xd800 && c <= 0xdfff) || c > 0x10ffff)
		return (0);
	else if (c <= 0x7F || (MB_CUR_MAX == 1 && c <= 0xFF))
		str[0] = c ? (char)c : 0;
	else if (c <= 0x7FF && MB_CUR_MAX >= 2)
	{
		str[0] = (c >> 6 | 0xC0) & 0xDF;
		str[1] = (c | 0x80) & 0xBF;
	}
	else if (c <= 0xFFFF && MB_CUR_MAX >= 3)
	{
		str[0] = (c >> 12 | 0xE0) & 0xEF;
		str[1] = (c >> 6 | 0x80) & 0xBF;
		str[2] = (c | 0x80) & 0xBF;
	}
	else if (c <= 0x10FFFF && MB_CUR_MAX >= 4)
	{
		str[0] = (c >> 18 | 0xF0) & 0xF7;
		str[1] = (c >> 12 | 0x80) & 0xBF;
		str[2] = (c >> 6 | 0x80) & 0xBF;
		str[3] = (c | 0x80) & 0xBF;
	}
	else
		return (0);
	return (1);
}

static char	ftpf_wchar_len(wchar_t c, t_par *p)
{
	char	len;

	if (c < -1 || c > 0x10ffff || (c >= 0xd800 && c <= 0xdfff))
		return (0);
	else if (c <= 0x7f || (MB_CUR_MAX == 1 && c <= 0xFF))
		len = 1;
	else if (c <= 0x7ff && MB_CUR_MAX >= 2)
		len = 2;
	else if (c <= 0xffff && MB_CUR_MAX >= 3)
		len = 3;
	else
		len = (c <= 0x10ffff && MB_CUR_MAX >= 4) ? 4 : 0;
	return (len >= p->precision ? len : 0);
}

int			ftpf_handle_wchar(t_par *p, va_list ap, t_buf *buf)
{
	wchar_t	c;
	char	tmp[5];
	size_t	len;

	c = va_arg(ap, wchar_t);
	ft_memset(tmp, 0, 5);
	if (p->flags & F_BIN)
		return (ftpf_handle_bin(&c, p, buf));
	else
	{
		if (!(len = ftpf_convert_wchar(c, tmp)))
			return (0);
//		len = ftpf_wchar_len(c, p);
		if ((p->flags & F_WIDTH || p->flags & F_ZERO) && p->width >= len)
			ftpf_buffer_fill(buf, p->flags & F_ZERO ? '0' : ' ',
				p->width - len);
		c == 0 ? ftpf_buffer_fill(buf, 0, 1)
			: ftpf_buffer_literal((const char*)tmp, buf);
		if (p->flags & F_MINUS && p->width >= len)
			ftpf_buffer_fill(buf, ' ', p->width - len);
	}
	return (1);
}

int			ftpf_handle_char(t_par *p, va_list ap, t_buf *buf)
{
	char	c;

	if (p->e_mod > NONE)
		return (ftpf_handle_wchar(p, ap, buf));
	c = (char)va_arg(ap, int);
	if (p->flags & F_BIN)
		return (ftpf_handle_bin(&c, p, buf));
	if ((p->flags & F_WIDTH || p->flags & F_ZERO) && p->width >= 1)
		ftpf_buffer_fill(buf, p->flags & F_ZERO ? '0' : ' ', p->width - 1);
	ftpf_buffer_fill(buf, c, 1);
	if (p->flags & F_MINUS)
		ftpf_buffer_fill(buf, ' ', p->width - 1);
	return (1);
}
