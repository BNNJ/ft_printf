/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_flags.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/28 15:28:41 by pfragnou          #+#    #+#             */
/*   Updated: 2018/06/28 15:28:42 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ftpf_get_format_flag(t_par *p, const char **format)
{
	int	i;

	while ((i = ft_findchar("0#+- &", **format)) >= 0)
	{
		p->flags |= (1 << i);
		++*format;
	}
	if (p->flags & F_SPACE)
		p->prefix[0] = ' ';
	if (p->flags & F_PLUS)
		p->prefix[0] = '+';

}

void	ftpf_get_width(t_par *p, const char **format, va_list ap)
{
	if (**format == '*')
	{
		p->width = va_arg(ap, int);
		++*format;
		p->flags |= F_WIDTH;
	}
	else if (**format >= '1' && **format <= '9')
	{
		p->width = ft_atoi(*format);
		while (**format >= '0' && **format <= '9')
			++*format;
		p->flags |= F_WIDTH;
	}
}

void	ftpf_get_precision(t_par *p, const char **format, va_list ap)
{
	if (**format == '.')
	{
		++*format;
		if (**format == '*')
		{
			p->precision = va_arg(ap, int);
			++*format;
		}
		else
		{
			p->precision = ft_atoi(*format);
			while (**format >= '0' && **format <= '9')
				++*format;
		}
		p->flags |= F_PRECI;
	}
}

void	ftpf_get_size_flag(t_par *p, const char **format)
{
	p->e_mod = NONE;
	while (**format == 'l' || **format == 'h'
		|| **format == 'j' || **format == 'z')
	{
		if (**format == 'l')
		{
			if (p->e_mod == L)
				p->e_mod = LL;
			else
				p->e_mod = p->e_mod <= NONE ? L : p->e_mod;
		}
		else if (**format == 'h')
		{
			if (p->e_mod == H)
				p->e_mod = HH;
			else
				p->e_mod = p->e_mod <= NONE ? L : p->e_mod;
		}
		else if (**format == 'j')
			p->e_mod = p->e_mod <= LL ? J : p->e_mod;
		else
			p->e_mod = Z;
		++*format;
	}
}

void	ftpf_get_type(t_par *p, const char **format)
{
	p->type = **format;
	++*format;
	if (p->type == 'x' || p->type == 'X' || p->type == 'p' || p->type == 'P')
	{
		p->base = 16;
		p->flags & F_HASH
			? ft_memcpy(p->prefix, p->type >= 'a' ? "0x" : "0X", 2)
			: 0;
	}
	else if (p->type == 'o' || p->type == 'O')
		p->base = 8;
	else if (p->type == 'b' || p->type == 'B')
	{
		p->base = 2;
		p->flags & F_HASH && p->type == 'b'? ft_memcpy(p->prefix, "0b", 2) : 0;
	}
	else
		p->base = 10;
	if ((ft_findchar("CSDOU", p->type) >= 0) && p->e_mod <= NONE)
		p->e_mod = L;
}
