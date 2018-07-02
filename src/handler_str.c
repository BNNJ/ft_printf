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

static size_t	ft_wstrlen(wchar_t *str)
{
	size_t	len;

	len = 0;
	if (str)
		while (*str)
		{
			if (*str < -1 || *str > 0x10ffff
				|| (*str >= 0xd800 && *str <= 0xdfff))
				return (0);
			else if (*str <= 0x7f)
				len += 1;
			else if (*str <= 0x7ff)
				len += 2;
			else if (*str <= 0xffff)
				len += 3;
			else if (*str <= 0x10ffff)
				len += 4;
			++str;
		}
	return (len);
}

static void		ftpf_buffer_wstr(wchar_t *str, t_par *p, t_buf *buf)
{
	char	tmp[5];
	size_t	i;
	size_t	k;

	i = 0;
	while (i <= p->precision)
	{
		k = 0;
		ft_memset(tmp, 0, 5);
		ftpf_convert_wchar(*str, tmp);
		while (*tmp & (128 >> k))
			++k;
		i += k;
		if (i > p->precision)
			break;
		ftpf_buffer_literal(tmp, buf);
		++str;
	}
}

int				ftpf_handle_wstr(t_par *p, va_list ap, t_buf *buf)
{
	wchar_t	*str;
	size_t	len;

	str = va_arg(ap, wchar_t*);
	if (!(len = ft_wstrlen(str)))
		return (0);
	p->precision = p->flags & F_PRECI && p->precision < len
		? p->precision : len;
	if (p->width > len && !(p->flags & F_MINUS))
		ftpf_buffer_fill(buf, ' ', p->width - p->precision);
	if (!(str))
		ftpf_buffer_copy("(null)", buf, p->precision);
	else
		ftpf_buffer_wstr(str, p, buf);
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
		ftpf_buffer_fill(buf, ' ', p->width - p->precision);
	ftpf_buffer_copy(str ? str : "(null)", buf, p->precision);
	if (p->width > p->precision && p->flags & F_MINUS)
		ftpf_buffer_fill(buf, ' ', p->width - p->precision);
	return (1);
}
