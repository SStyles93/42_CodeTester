#include "../../include/tester.h"
#include "../../include/weak_functions.h"
#include <stdio.h>

void	test_ft_printf(t_ctx *ctx)
{
	check_int(ctx, "\"42\"", printf("%c", 'c'), ft_printf("%c", 'c'));
}
