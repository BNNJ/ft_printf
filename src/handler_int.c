/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_int.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/29 14:10:12 by pfragnou          #+#    #+#             */
/*   Updated: 2018/06/29 14:10:14 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
** Sets the precision to be used in umaxtoa_base() by first picking either
** p->precision or len (the number of digits in nb) depending on which is bigger
** then adds 1 for octal base if F_HASH has been enabled.
** Of course, zero being an exception, we get this mess of conditionals.
** Then, disables width flags depending on where we want the padding to be added
** F_WIDTH : padding, prefix, number
** F_ZERO : prefix, padding, number
** F_MINUS : prefix, number, padding
** This is only done to make ftpf_handle_int clearer.
** Don't ask me why i handled it like that instead of using the prefix like
** the other bases : The man says it's done like that, so i did it like that.
*/

static size_t	ftpf_int_setup(uintmax_t nb, t_par *p)
{
	size_t		len;
	uintmax_t	tmp;

	len = 1;
	tmp = nb;
	while (tmp /= p->base)
		++len;
	if (!(p->flags & F_PRECI))
		p->precision = len;
	else if (p->precision != 0 || nb != 0)
		p->precision = p->precision > len ? p->precision : len;
	p->precision += !(p->flags & F_PRECI) && nb == 0 ? 0
		: p->flags & F_HASH && len >= p->precision
		&& (p->type == 'o' || p->type == 'O');
	tmp = nb ? p->precision + ft_strlen(p->prefix) : p->precision;
	p->width = p->width > tmp ? p->width - tmp : 0;
	if (p->flags & F_ZERO
		&& (p->flags & F_PRECI || !(p->flags & F_WIDTH) || p->flags & F_MINUS))
		p->flags &= ~F_ZERO;
	if (p->flags & F_MINUS && !(p->flags & F_WIDTH))
		p->flags &= ~F_MINUS;
	if (p->flags & F_ZERO || p->flags & F_MINUS)
		p->flags &= ~F_WIDTH;
	return (len);
}

int				ftpf_handle_int(t_par *p, va_list ap, t_buf *buf)
{
	uintmax_t	nb;
	size_t		len;

	nb = (ft_findchar("oOuUxXb", p->type) >= 0 || p->flags & F_BIN)
		? ftpf_convert_unsigned(p, ap) : ftpf_convert_int(p, ap);
	if (p->flags & F_BIN)
		return (ftpf_handle_bin(&nb, p, buf));
	len = ftpf_int_setup(nb, p);
	printf("%zd\n", p->width);
	if (p->flags & F_WIDTH)
		ftpf_buffer_fill(buf, ' ', p->width);
	if (nb != 0)
		ftpf_buffer_literal(p->prefix, buf);
	if (p->flags & F_ZERO)
		ftpf_buffer_fill(buf, '0', p->width);
	ftpf_umaxtoa_base(nb, len, p, buf);
	if (p->flags & F_MINUS)
		ftpf_buffer_fill(buf, ' ', p->width);
	return (1);
}
