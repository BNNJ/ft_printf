
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <wchar.h>
# include <stdint.h>
# include <stdio.h>

int		main()
{
	printf("%lu\n", sizeof(uintmax_t) * 8);
	return 0;
}