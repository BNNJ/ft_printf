#include "ft_printf.h"

int		ftpf_strjoin(t_buf *buf)
{
	char	*new_str;

	if (!(new_str = (char*)malloc(buf->ret + buf->cursor)))
		return (0);
	ft_memcpy(new_str, buf->str, buf->ret);
	ft_memcpy(new_str + buf->ret, buf->cursor);
	free(buf->str);
	buf->str = new_str;
	
}