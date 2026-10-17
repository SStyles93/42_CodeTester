#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

static void	t_putendl(t_ctx *ctx, const char *label, const char *s,
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
	ft_putendl_fd((char *)s, pfd[1]);
	close(pfd[1]);
	n = (int)read(pfd[0], buf, (int)sizeof(buf) - 1);
	buf[n < 0 ? 0 : n] = '\0';
	close(pfd[0]);
	check_str(ctx, label, expected, buf);
}

void	test_ft_putendl_fd(t_ctx *ctx)
{
	/* basic — should append '\n' */
	t_putendl(ctx, "putendl_fd \"hello\"", "hello", "hello\n");

	/* empty string — should still write '\n' */
	t_putendl(ctx, "putendl_fd \"\"", "", "\n");

	/* string already ending with newline — gets a second '\n' */
	t_putendl(ctx, "putendl_fd \"hi\\n\"", "hi\n", "hi\n\n");
}
