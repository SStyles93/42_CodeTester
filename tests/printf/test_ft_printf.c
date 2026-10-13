#include "../../include/tester.h"
#include <stdio.h>

int ft_printf(const char *, ...);

void	test_ft_printf(t_ctx *ctx)
{
	check_int(ctx, "\"42\"", printf("%c", 'c'), ft_printf("%c", 'c'));
}
