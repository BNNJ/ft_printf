#include "ft_printf.h"
#include <locale.h>

int	main()
{
	setlocale(LC_ALL, "");

	int		i = 9;
	float	d = 5.0;

//	ft_printf("ft: Hey %18.8S, %3% ho %.8C Let's go\n", L"我是一只猫。", 0x1234);
//	printf("pf: Hey %18.8S, %3% ho %.8C Let's go\n", L"我是一只猫。", 0x1234);
	ft_printf("ft : % x|%+x\n", 42, 42);
	printf("pf : % x|%+x\n", 42, 42);
	return 0;
}