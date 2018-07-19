/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfragnou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/19 02:21:51 by pfragnou          #+#    #+#             */
/*   Updated: 2018/07/19 02:21:52 by pfragnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_asprintf(char **str, const char *format, ...)
{
	t_buf	buf;
	int		ret;
	va_list	ap;

	if (!(*str = (char*)malloc(0)))
		return (-1);
	ft_memset(&buf, 0, sizeof(buf));
	buf.strmode = 1;
	buf.fd = 1;
	buf.str = str;
	va_start(ap, format);
	ret = ftpf_groundcontrol(format, ap, &buf);
	va_end(ap);
	*str = *buf.str;
	return (ret);
}

int		ft_sprintf(char *str, const char *format, ...)
{
	va_list	ap;
	t_buf	buf;
	int		ret;

	ft_memset(&buf, 0, sizeof(buf));
	buf.strmode = 2;
	buf.fd = 1;
	buf.str = &str;
	va_start(ap, format);
	ret = ftpf_groundcontrol(format, ap, &buf);
	va_end(ap);
	return (ret);
}
