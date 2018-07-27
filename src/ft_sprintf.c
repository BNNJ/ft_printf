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

/*
** allocate a string to store the formated output into
*/

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

/*
** store the formated string into a string given as parameter.
** The string must be big enough to hold everything.
*/

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

int		ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list	ap;
	t_buf	buf;
	int		ret;

	ft_memset(&buf, 0, sizeof(buf));
	buf.strmode = 3;
	buf.fd = 1;
	buf.str = &str;
	buf.max_len = size;
	va_start(ap, format);
	ret = ftpf_groundcontrol(format, ap, &buf);
	va_end(ap);
	return (ret);
}
