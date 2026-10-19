#include "gnl_tester.h"

/*
** Multi-call behaviour: repeated calls must return one line per call and
** eventually return NULL when the source is exhausted.
*/
void	test_gnl_lines(t_ctx *ctx)
{
	char	*line;
	int		fd;

	/* two lines, both newline-terminated */
	fd = gnl_open_pipe("line1\nline2\n");
	if (fd >= 0)
	{
		line = get_next_line(fd);
		check_str(ctx, "2-line call 1", "line1\n", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "2-line call 2", "line2\n", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "2-line call 3 → NULL", NULL, line);
		free(line);
		close(fd);
	}

	/* three lines, last one has no trailing newline */
	fd = gnl_open_pipe("first\nsecond\nthird");
	if (fd >= 0)
	{
		line = get_next_line(fd);
		check_str(ctx, "3-line call 1", "first\n", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "3-line call 2", "second\n", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "3-line call 3 (no \\n at EOF)", "third", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "3-line call 4 → NULL", NULL, line);
		free(line);
		close(fd);
	}

	/* three consecutive newlines: each call must return exactly "\n" */
	fd = gnl_open_pipe("\n\n\n");
	if (fd >= 0)
	{
		line = get_next_line(fd);
		check_str(ctx, "3 newlines call 1", "\n", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "3 newlines call 2", "\n", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "3 newlines call 3", "\n", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "3 newlines call 4 → NULL", NULL, line);
		free(line);
		close(fd);
	}

	/* empty line in the middle: the blank line must be returned as "\n" */
	fd = gnl_open_pipe("a\n\nb\n");
	if (fd >= 0)
	{
		line = get_next_line(fd);
		check_str(ctx, "mid-empty call 1", "a\n", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "mid-empty call 2 (blank line)", "\n", line);
		free(line);
		line = get_next_line(fd);
		check_str(ctx, "mid-empty call 3", "b\n", line);
		free(line);
		gnl_drain(fd);
		close(fd);
	}
}
