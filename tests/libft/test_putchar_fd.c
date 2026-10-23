#include "../../include/tester.h"

#include "../../include/weak_functions.h"

/* Write c to a fresh pipe and read back what arrived. */
static void	t_putchar(t_ctx *ctx, const char *label, char c,
	const char *expected)
{
	int		pfd[2];
	char	buf[16];
	int		n;

	if (pipe(pfd) < 0)
	{
		check_ok(ctx, label, 0, "pipe() failed");
		return ;
	}
	ft_putchar_fd(c, pfd[1]);
	close(pfd[1]);
	n = (int)read(pfd[0], buf, (int)sizeof(buf) - 1);
	buf[n < 0 ? 0 : n] = '\0';
	close(pfd[0]);
	check_str(ctx, label, expected, buf);
}

void	test_ft_putchar_fd(t_ctx *ctx)
{
	t_putchar(ctx, "putchar_fd 'A'",  'A',  "A");
	t_putchar(ctx, "putchar_fd '\\n'", '\n', "\n");
}
