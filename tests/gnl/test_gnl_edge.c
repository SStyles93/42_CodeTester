#include "gnl_tester.h"

/*
** Edge cases: a leading newline, a line much longer than BUFFER_SIZE, and a
** single-character line.  These expose common off-by-one errors and incorrect
** buffer-accumulation logic.
*/
void	test_gnl_edge(t_ctx *ctx)
{
	char	*line;
	int		fd;

	/* newline as the very first character */
	fd = gnl_open_pipe("\nhello\n");
	if (fd >= 0)
	{
		line = g_gnl(fd);
		check_str(ctx, "leading \\n call 1", "\n", line);
		free(line);
		line = g_gnl(fd);
		check_str(ctx, "leading \\n call 2", "hello\n", line);
		free(line);
		gnl_drain(fd);
		close(fd);
	}

	/* single character followed by newline */
	fd = gnl_open_pipe("x\n");
	if (fd >= 0)
	{
		line = g_gnl(fd);
		check_str(ctx, "single-char + \\n", "x\n", line);
		free(line);
		gnl_drain(fd);
		close(fd);
	}

	/* single character without newline at EOF */
	fd = gnl_open_pipe("z");
	if (fd >= 0)
	{
		line = g_gnl(fd);
		check_str(ctx, "single-char no \\n", "z", line);
		free(line);
		gnl_drain(fd);
		close(fd);
	}

	/*
	** Line much longer than any typical BUFFER_SIZE (2048 'A' + newline).
	** A correct implementation must keep reading until it finds the newline
	** regardless of how many read() calls that takes.
	*/
	{
		const size_t	len = 2048;
		char			*big;
		char			*expected;

		big = malloc(len + 2);
		expected = malloc(len + 2);
		if (big && expected)
		{
			memset(big, 'A', len);
			big[len] = '\n';
			big[len + 1] = '\0';
			memset(expected, 'A', len);
			expected[len] = '\n';
			expected[len + 1] = '\0';
			fd = gnl_open_pipe(big);
			if (fd >= 0)
			{
				line = g_gnl(fd);
				check_str(ctx, "2048-char line (> BUFFER_SIZE)", expected, line);
				free(line);
				gnl_drain(fd);
				close(fd);
			}
		}
		free(big);
		free(expected);
	}

	/* newline immediately followed by EOF with no further content */
	fd = gnl_open_pipe("last\n");
	if (fd >= 0)
	{
		line = g_gnl(fd);
		check_str(ctx, "line+\\n then EOF call 1", "last\n", line);
		free(line);
		line = g_gnl(fd);
		check_str(ctx, "line+\\n then EOF call 2", NULL, line);
		free(line);
		close(fd);
	}
}
