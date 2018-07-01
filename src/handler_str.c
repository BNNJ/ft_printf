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
		while (str[len])
			++len;
	return (len);
}

static size_t	get_precision(char *str, t_par *p)
{
	size_t	i;
	size_t	k;

	i = 0;
	while (i <= p->precision)
	{
		k = 0;
		while (str[i] & (128 >> k))
			++k;
		if (i + k > p->precision)
			return (i);
		i += k;
	}
	return (i);
}

int				ftpf_handle_wstr(t_par *p, va_list ap, t_buf *buf)
{
	wchar_t	*str;

	str = va_arg(ap, wchar_t*);
	len = str ? ft_wstrlen(str) : 6;
	
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
