/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter_int.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/29 14:10:22 by pfragnou          #+#    #+#             */
/*   Updated: 2018/06/29 14:10:24 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		ftpf_umaxtoa_base(uintmax_t nb, size_t len, t_par *p, t_buf *buf)
{
	int			i;
	char		buffer[UINTMAX_WIDTH];
	char		base[16];
	
	i = UINTMAX_WIDTH;
	ft_memcpy(base, p->type >= 'A' && p->type <= 'Z' ? UCBASE : LCBASE, 16);
	if (p->precision > len)
	{
		ftpf_buffer_fill(buf, '0', p->precision - len);
		p->precision -= (p->precision - len);
	}
	while (p->precision)
	{
		buffer[--i] = base[nb % p->base];
		nb /= p->base;
		--p->precision;
	}
	ftpf_buffer_copy(buffer + i, buf, UINTMAX_WIDTH - i);
}

uintmax_t	ftpf_convert_int(t_par *p, va_list ap)
{
	intmax_t	nb;
	uintmax_t	unb;

	nb = 0;
	if (p->e_mod == HH)
		nb = (char)va_arg(ap, int);
	else if (p->e_mod == H)
		nb = (short)va_arg(ap, int);
	else if (p->e_mod == NONE)
		nb = va_arg(ap, int);
	else if (p->e_mod == L)
		nb = (long)va_arg(ap, intmax_t);
	else if (p->e_mod == LL)
		nb = (long long)va_arg(ap, intmax_t);
	else if (p->e_mod == Z)
		nb = (ssize_t)va_arg(ap, intmax_t);
	else if (p->e_mod == J)
		nb = va_arg(ap, intmax_t);
	unb = (nb >= 0) ? nb : -nb;
	p->prefix[0] = nb < 0 ? '-' : p->prefix[0];
	return (unb);
}

uintmax_t	ftpf_convert_unsigned(t_par *p, va_list ap)
{
	uintmax_t	nb;

	nb = 0;
	if (p->e_mod == HH)
		nb = (unsigned char)va_arg(ap, unsigned int);
	else if (p->e_mod == H)
		nb = (unsigned short)va_arg(ap, unsigned int);
	else if (p->e_mod == NONE)
		nb = va_arg(ap, unsigned int);
	else if (p->e_mod == L)
		nb = (unsigned long)va_arg(ap, uintmax_t);
	else if (p->e_mod == LL)
		nb = (unsigned long long)va_arg(ap, uintmax_t);
	else if (p->e_mod == Z)
		nb = (size_t)va_arg(ap, uintmax_t);
	else if (p->e_mod == J)
		nb = va_arg(ap, uintmax_t);
	return (nb);	
}
