#include "ft_printf.h"

int	main()
{

//	ft_printf("ft: Hey %18.8S, %3% ho %.8C Let's go\n", L"我是一只猫。", 0x1234);
//	printf("pf: Hey %18.8S, %3% ho %.8C Let's go\n", L"我是一只猫。", 0x1234);
//	ft_printf("ft : % x|%+x\n", 42, 42);
	ft_printf("%{red:italic:bold:underline} blabla %{reset:blue} blabla\n");
	return 0;
}