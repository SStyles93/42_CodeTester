#include "../../include/tester.h"

/*
** NOTES on ft_putchar_fd.c:
**   - Uses '#include <libft.h>' (angle brackets) — should be '"libft.h"'.
**     Compiles only when the Libft directory is on the include path.
**   - Implementation itself is correct.
*/

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
		ctx->fail++;
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
	t_putchar(ctx, "putchar_fd 'z'",  'z',  "z");
	t_putchar(ctx, "putchar_fd '0'",  '0',  "0");
	t_putchar(ctx, "putchar_fd '!'",  '!',  "!");
	t_putchar(ctx, "putchar_fd '\\n'", '\n', "\n");
	t_putchar(ctx, "putchar_fd ' '",  ' ',  " ");
	t_putchar(ctx, "putchar_fd '1'",  '1',  "1");

	/* write to stderr (fd 2) — just make sure it doesn't crash */
	{
		int	pfd[2];

		if (pipe(pfd) >= 0)
		{
			ft_putchar_fd('X', pfd[1]);
			close(pfd[1]);
			close(pfd[0]);
		}
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET
				" putchar_fd to pipe fd didn't crash\n");
	}

	/* more special and control characters */
	t_putchar(ctx, "putchar_fd '\\t'",        '\t',  "\t");
	t_putchar(ctx, "putchar_fd '\\r'",        '\r',  "\r");
	t_putchar(ctx, "putchar_fd '~' (126)",    '~',   "~");
	t_putchar(ctx, "putchar_fd '\\x01' (SOH)", 0x01, "\x01");
	/* all printable ASCII boundary chars */
	t_putchar(ctx, "putchar_fd ' ' (32)",   ' ',  " ");
	t_putchar(ctx, "putchar_fd '\"' (34)",  '"',  "\"");
	t_putchar(ctx, "putchar_fd '/' (47)",   '/',  "/");
	t_putchar(ctx, "putchar_fd '@' (64)",   '@',  "@");
}
