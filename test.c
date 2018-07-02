#include "ft_printf.h"
#include <locale.h>

int		main()
{
	setlocale(LC_ALL, "");
//	int		i = 42;
//	wchar_t	c = 0x1234;
	wchar_t	*bla = NULL;
	wchar_t	*wstr = L"我是一只猫。";
	char	str[] = "blabla";
	float	f = -42.21;

	ft_printf("## %4.6k ##\n", 'f');
	printf("$$ %4.6k $$\n", 'f');
	return 0;
}

// 我是一只猫。