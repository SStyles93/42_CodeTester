#include "../../include/tester.h"
#include <string.h>
#include <limits.h>

/*
** NOTES on ft_putnbr_fd.c:
**   - Implementation is logically correct.
**   - The recursive call passes 'nbr/10' (unsigned int) to a parameter
**     declared as 'int' — implicit conversion, safe for values that fit.
**   - '#include "libft.h"' needs the Libft directory on the include path.
*/

#include "../../include/weak_functions.h"

static void	t_putnbr(t_ctx *ctx, const char *label, int n, const char *expected)
{
	int		pfd[2];
	char	buf[32];
	int		r;

	if (pipe(pfd) < 0)
	{
		ctx->fail++;
		return ;
	}
	ft_putnbr_fd(n, pfd[1]);
	close(pfd[1]);
	r = (int)read(pfd[0], buf, (int)sizeof(buf) - 1);
	buf[r < 0 ? 0 : r] = '\0';
	close(pfd[0]);
	check_str(ctx, label, expected, buf);
}

void	test_ft_putnbr_fd(t_ctx *ctx)
{
	t_putnbr(ctx, "putnbr_fd 0",          0,           "0");
	t_putnbr(ctx, "putnbr_fd 9",          9,           "9");
	t_putnbr(ctx, "putnbr_fd -9",         -9,          "-9");
	/* digit-count transitions */
	t_putnbr(ctx, "putnbr_fd 10",         10,          "10");
	t_putnbr(ctx, "putnbr_fd -10",        -10,         "-10");
	t_putnbr(ctx, "putnbr_fd 1000",       1000,        "1000");
	t_putnbr(ctx, "putnbr_fd 2147483647", 2147483647,  "2147483647");
	t_putnbr(ctx, "putnbr_fd INT_MIN",    -2147483648, "-2147483648");
}
