#include "gnl_tester.h"

/*
** Basic single-call behaviour: invalid fd, empty input, single line with and
** without a terminating newline, and a bare newline.
*/
void	test_get_next_line(t_ctx *ctx)
{
	char	*line;
	int		fd;

	/* invalid fd must return NULL without crashing */
	line = get_next_line(-1);
	check_str(ctx, "fd=-1 → NULL", NULL, line);
	free(line);

	/* pipe closed with no bytes written → immediate EOF */
	fd = gnl_open_pipe("");
	if (fd >= 0)
	{
		line = get_next_line(fd);
		check_str(ctx, "empty input → NULL", NULL, line);
		free(line);
		close(fd);
	}

	/* one line terminated by newline */
	fd = gnl_open_pipe("hello\n");
	if (fd >= 0)
	{
		line = get_next_line(fd);
		check_str(ctx, "\"hello\\n\" call 1", "hello\n", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "\"hello\\n\" call 2 → NULL", NULL, line);
		free(line);
		close(fd);
	}

	/* line at EOF without trailing newline */
	fd = gnl_open_pipe("hello");
	if (fd >= 0)
	{
		line = get_next_line(fd);
		check_str(ctx, "\"hello\" (no \\n) call 1", "hello", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "\"hello\" call 2 → NULL", NULL, line);
		free(line);
		close(fd);
	}

	/* just a newline character */
	fd = gnl_open_pipe("\n");
	if (fd >= 0)
	{
		line = get_next_line(fd);
		check_str(ctx, "\"\\n\" call 1", "\n", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "\"\\n\" call 2 → NULL", NULL, line);
		free(line);
		close(fd);
	}

	/* spaces and tabs are part of the line, not stripped */
	fd = gnl_open_pipe("  hello\t42  \n");
	if (fd >= 0)
	{
		line = get_next_line(fd);
		check_str(ctx, "spaces/tabs preserved", "  hello\t42  \n", line);
		free(line);
		gnl_drain(fd);
		close(fd);
	}
}
