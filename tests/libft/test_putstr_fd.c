#include "../../include/tester.h"
#include <string.h>

/*
** BUGS in ft_putstr_fd.c:
**   1. 'while(*s++)' increments s BEFORE writing — ft_putchar_fd receives
**      s[1] on the first iteration, so the first character is always skipped.
**      Fix: use '*s' then 's++' separately, or a classic indexed loop.
**   2. '#include <libft.h>' uses angle brackets — needs '"libft.h"' or
**      the Libft directory on the include path.
*/

void	ft_putstr_fd(char *s, int fd);

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
	/* basic string — BUG: first char is skipped, so "hello" → "ello" */
	t_putstr(ctx, "putstr_fd \"hello\"", "hello", "hello");

	/* single char */
	t_putstr(ctx, "putstr_fd \"a\"", "a", "a");

	/* empty string */
	t_putstr(ctx, "putstr_fd \"\"", "", "");

	/* string with spaces */
	t_putstr(ctx, "putstr_fd \"hello world\"", "hello world", "hello world");

	/* string with newline */
	t_putstr(ctx, "putstr_fd \"hi\\n\"", "hi\n", "hi\n");

	/* numeric string */
	t_putstr(ctx, "putstr_fd \"42\"", "42", "42");

	/* longer string */
	t_putstr(ctx, "putstr_fd long",
		"Lorem ipsum dolor sit amet",
		"Lorem ipsum dolor sit amet");

	/* tab in string */
	t_putstr(ctx, "putstr_fd with tab", "hel\tlo", "hel\tlo");
	/* only special chars */
	t_putstr(ctx, "putstr_fd special !@#$", "!@#$", "!@#$");
	/* multiple newlines */
	t_putstr(ctx, "putstr_fd multi-newline", "a\nb\nc", "a\nb\nc");
	/* all digits */
	t_putstr(ctx, "putstr_fd all digits", "0123456789", "0123456789");
}
