/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_ptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 21:21:21 by pfragnou          #+#    #+#             */
/*   Updated: 2018/06/30 21:21:23 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ftpf_handle_ptr(t_par *p, va_list ap, t_buf *buf)
{
	void			*ptr;
	size_t			len;
	unsigned long	tmp;

	len = 1;
	ptr = va_arg(ap, void*);
	tmp = (unsigned long)ptr;
	while (tmp /= 16)
		++len;
	p->precision = p->precision > len || (p->flags & F_PRECI && !ptr)
		? p->precision : len;
	p->width = p->width > len + 2 ? p->width - len - 2 : 0;
	if (p->flags & F_WIDTH)
		ftpf_buffer_fill(buf, ' ', p->width);
	ftpf_buffer_copy(p->type == 'p' ? "0x" : "0X", buf, 2);
	if (p->flags & F_ZERO)
		ftpf_buffer_fill(buf, '0', p->width);
	ftpf_umaxtoa_base((unsigned long)ptr, len, p, buf);
	if (p->flags & F_MINUS)
		ftpf_buffer_fill(buf, ' ', p->width);
	return (1);
}
