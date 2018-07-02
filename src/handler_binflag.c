/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_binflag.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/28 20:00:10 by pfragnou          #+#    #+#             */
/*   Updated: 2018/06/28 20:00:12 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ftpf_handle_bin(void *ptr, t_par *p, t_buf *buf)
{
	int	bits;
	
	if (p->type == 'c' || (p->type == 'C' && p->e_mod == H) || p->e_mod == HH)
		bits = 8;
	else if (p->e_mod == H && p->type != 'F' && p->type != 'f')
		bits = 16;
	else if ((p->e_mod == NONE && p->type != 'F') || p->type == 'C')
		bits = 32;
	else
		bits = 64;
	while (bits--)
	{
		*(unsigned long long*)ptr & (1LL << bits)
			? ftpf_buffer_fill(buf, '1', 1)
			: ftpf_buffer_fill(buf, '0', 1);
		if (p->type == 'f' && (bits == 31 || bits == 23) && p->flags & F_HASH)
			ftpf_buffer_fill(buf, ' ', 1);
		else if (p->type == 'F' && (bits == 63 || bits == 52)
			&& p->flags & F_HASH)
			ftpf_buffer_fill(buf, ' ', 1);
		else if (bits % 8 == 0 && bits != 0)
			ftpf_buffer_fill(buf, ' ', 1);
	}
	return (1);
}
