#include "gnl_tester.h"
#ifndef _WIN32
# include <fcntl.h>
#endif

/*
** "Make sure that your function works as expected both when reading a file
** and when reading from the standard input." / "A file descriptor does not
** only point to regular files." Plus the error cases that must return NULL.
*/

static void	check_regular_file(t_ctx *ctx)
{
	char	*line;
	int		fd;

	fd = gnl_open_file("file line 1\nfile line 2\nno newline");
	if (fd < 0)
		return ;
	line = g_gnl(fd);
	check_str(ctx, "regular file call 1", "file line 1\n", line);
	free(line);
	line = g_gnl(fd);
	check_str(ctx, "regular file call 2", "file line 2\n", line);
	free(line);
	line = g_gnl(fd);
	check_str(ctx, "regular file call 3 (no \\n at EOF)", "no newline", line);
	free(line);
	line = g_gnl(fd);
	check_str(ctx, "regular file call 4 → NULL", NULL, line);
	free(line);
	close(fd);
}

/* stdin (fd 0) is temporarily replaced by a pipe. */
static void	check_stdin(t_ctx *ctx)
{
	char	*line;
	int		saved;
	int		fd;

	saved = dup(0);
	fd = gnl_open_pipe("from stdin 1\nfrom stdin 2\n");
	if (saved < 0 || fd < 0 || dup2(fd, 0) < 0)
	{
		if (fd >= 0)
			close(fd);
		if (saved >= 0)
			close(saved);
		return ;
	}
	close(fd);
	line = g_gnl(0);
	check_str(ctx, "stdin call 1", "from stdin 1\n", line);
	free(line);
	line = g_gnl(0);
	check_str(ctx, "stdin call 2", "from stdin 2\n", line);
	free(line);
	line = g_gnl(0);
	check_str(ctx, "stdin call 3 → NULL", NULL, line);
	free(line);
	gnl_drain(0);
	dup2(saved, 0);
	close(saved);
}

/* After EOF, every further call keeps returning NULL. */
static void	check_after_eof(t_ctx *ctx)
{
	char	*line;
	int		fd;

	fd = gnl_open_pipe("only\n");
	if (fd < 0)
		return ;
	free(g_gnl(fd));
	line = g_gnl(fd);
	check_str(ctx, "after EOF call 1 → NULL", NULL, line);
	free(line);
	line = g_gnl(fd);
	check_str(ctx, "after EOF call 2 → NULL", NULL, line);
	free(line);
	close(fd);
}

static void	check_invalid_fds(t_ctx *ctx)
{
	char	*line;
	int		fd;

	line = g_gnl(-1);
	check_str(ctx, "fd=-1 → NULL", NULL, line);
	free(line);
	line = g_gnl(4242);
	check_str(ctx, "fd=4242 (never opened) → NULL", NULL, line);
	free(line);
	fd = gnl_open_pipe("closed\n");
	if (fd >= 0)
	{
		close(fd);
		line = g_gnl(fd);
		check_str(ctx, "closed fd → NULL", NULL, line);
		free(line);
	}
#ifndef _WIN32
	fd = open(".", O_RDONLY);
	if (fd >= 0)
	{
		line = g_gnl(fd);
		check_str(ctx, "directory fd (read() fails) → NULL", NULL, line);
		free(line);
		close(fd);
	}
#endif
}

/*
** read() error in the middle of a file: the fd is closed after one line,
** get_next_line() must return NULL, and a new file opened on the same fd
** number must be read from its start, not from the old file's leftovers.
*/
static void	check_read_error(t_ctx *ctx)
{
	char	*line;
	int		fd;
	int		other;

	fd = gnl_open_file("old 1\nold 2\nold 3\n");
	if (fd < 0)
		return ;
	line = g_gnl(fd);
	check_str(ctx, "read error: first line", "old 1\n", line);
	free(line);
	close(fd);
	line = g_gnl(fd);
	check_str(ctx, "read error: closed mid-file → NULL", NULL, line);
	free(line);
	other = gnl_open_file("new 1\nnew 2\n");
	if (other < 0)
		return ;
	if (other != fd)
	{
		dup2(other, fd);
		close(other);
	}
	line = g_gnl(fd);
	check_str(ctx, "read error: reopened fd starts fresh", "new 1\n", line);
	free(line);
	gnl_drain(fd);
	close(fd);
}

void	test_gnl_sources(t_ctx *ctx)
{
	check_regular_file(ctx);
	check_stdin(ctx);
	check_after_eof(ctx);
	check_invalid_fds(ctx);
	check_read_error(ctx);
}
