#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

static void	t_putstr(t_ctx *ctx, const char *label, const char *s,
	const char *expected)
{
	int		pfd[2];
	char	buf[256];
	int		n;

	if (pipe(pfd) < 0)
	{
		ctx->fail++;
		return ;
	}
	ft_putstr_fd((char *)s, pfd[1]);
	close(pfd[1]);
	n = (int)read(pfd[0], buf, (int)sizeof(buf) - 1);
	buf[n < 0 ? 0 : n] = '\0';
	close(pfd[0]);
	check_str(ctx, label, expected, buf);
}

void	test_ft_putstr_fd(t_ctx *ctx)
{
	t_putstr(ctx, "putstr_fd \"hello\"", "hello", "hello");

	/* empty string */
	t_putstr(ctx, "putstr_fd \"\"", "", "");
}
