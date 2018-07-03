/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/01 14:50:06 by pfragnou          #+#    #+#             */
/*   Updated: 2018/07/01 14:50:08 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	ft_wstrlen(wchar_t *str, t_par *p)
{
	size_t	len;
	int		k;

	len = 0;
	if (str)
		while (*str)
		{
			k = 0;
			if (*str < -1 || *str > 0x10ffff
				|| (*str >= 0xd800 && *str <= 0xdfff))
				return (0);
			else if (*str <= 0x7f || (MB_CUR_MAX == 1 && *str <= 0xFF))
				k = 1;
			else if (*str <= 0x7ff && MB_CUR_MAX >= 2)
				k = 2;
			else if (*str <= 0xffff && MB_CUR_MAX >= 3)
				k = 3;
			else
				k = (*str <= 0x10ffff && MB_CUR_MAX >= 4) ? 4 : 0;
			if (p->flags & F_PRECI && len + k > p->precision)
				return (len);
			len += k;
			++str;
		}
	return (len);
}

static int		ftpf_buffer_wstr(wchar_t *str, t_par *p, t_buf *buf)
{
	char	tmp[5];
	size_t	i;

	i = 0;
	while (i <= p->precision && *str)
	{
		ft_memset(tmp, 0, 5);
		if (!(ftpf_convert_wchar(*str, tmp)))
			return (ftpf_handle_error(buf, i));
		i += ft_strlen(tmp);
		if (i > p->precision)
			break ;
		ftpf_buffer_literal(tmp, buf);
		++str;
	}
	return (1);
}

int				ftpf_handle_wstr(t_par *p, va_list ap, t_buf *buf)
{
	wchar_t	*str;
	size_t	len;

	str = va_arg(ap, wchar_t*);
	len = str ? ft_wstrlen(str, p) : 6;
	p->precision = p->flags & F_PRECI && p->precision < len
		? p->precision : len;
	if (p->width > p->precision && !(p->flags & F_MINUS))
		ftpf_buffer_fill(buf, p->flags & F_ZERO ? '0' : ' ',
			p->width - p->precision);
	if (!(str))
		ftpf_buffer_copy("(null)", buf, p->precision);
	else if (!(ftpf_buffer_wstr(str, p, buf)))
	{
		return (ftpf_handle_error(buf,
			p->width > len && !(p->flags & F_MINUS) ? p->width : 0));
	}
	if (p->width > len && p->flags & F_MINUS)
		ftpf_buffer_fill(buf, ' ', p->width - p->precision);
	return (1);
}

int				ftpf_handle_str(t_par *p, va_list ap, t_buf *buf)
{
	char	*str;
	size_t	len;

	if (p->e_mod > NONE)
		return (ftpf_handle_wstr(p, ap, buf));
	str = va_arg(ap, char*);
	len = str ? ft_strlen(str) : 6;
	p->precision = p->flags & F_PRECI && p->precision < len
		? p->precision : len;
	if (p->width > p->precision && !(p->flags & F_MINUS))
		ftpf_buffer_fill(buf, p->flags & F_ZERO ? '0' : ' ',
			p->width - p->precision);
	ftpf_buffer_copy(str ? str : "(null)", buf, p->precision);
	if (p->width > p->precision && p->flags & F_MINUS)
		ftpf_buffer_fill(buf, ' ', p->width - p->precision);
	return (1);
}
