#include "gnl_tester.h"

/*
	Two file descriptors read in alternating order.
	Each call must return the next line of the fd that was passed, not the last
	fd read from.
*/
void	test_gnl_two_fd(t_ctx *ctx)
{
	int		fd1;
	int		fd2;
	char	*line;

	fd1 = gnl_open_pipe("alpha\nbeta\n");
	fd2 = gnl_open_pipe("uno\ndos\n");
	if (fd1 < 0 || fd2 < 0)
	{
		if (fd1 >= 0)
			close(fd1);
		if (fd2 >= 0)
			close(fd2);
		return ;
	}

	line = g_gnl(fd1);
	check_str(ctx, "fd1 call 1", "alpha\n", line);
	free(line);

	line = g_gnl(fd2);
	check_str(ctx, "fd2 call 1 (not fd1)", "uno\n", line);
	free(line);

	line = g_gnl(fd1);
	check_str(ctx, "fd1 call 2 (resumed)", "beta\n", line);
	free(line);

	line = g_gnl(fd2);
	check_str(ctx, "fd2 call 2 (resumed)", "dos\n", line);
	free(line);

	line = g_gnl(fd1);
	check_str(ctx, "fd1 call 3", NULL, line);
	free(line);

	line = g_gnl(fd2);
	check_str(ctx, "fd2 call 3", NULL, line);
	free(line);

	close(fd1);
	close(fd2);
}

/*
	Three file descriptors cycled in rotation.  Exhausting fd3 early must not
	corrupt the state of fd1 or fd2.
*/
void	test_gnl_three_fd(t_ctx *ctx)
{
	int		fd1;
	int		fd2;
	int		fd3;
	char	*line;

	fd1 = gnl_open_pipe("line_A\nline_B\n");
	fd2 = gnl_open_pipe("line_C\nline_D\n");
	fd3 = gnl_open_pipe("line_E\n");
	if (fd1 < 0 || fd2 < 0 || fd3 < 0)
	{
		if (fd1 >= 0)
			close(fd1);
		if (fd2 >= 0)
			close(fd2);
		if (fd3 >= 0)
			close(fd3);
		return ;
	}

	line = g_gnl(fd1);
	check_str(ctx, "fd1 call 1", "line_A\n", line);
	free(line);

	line = g_gnl(fd2);
	check_str(ctx, "fd2 call 1", "line_C\n", line);
	free(line);

	line = g_gnl(fd3);
	check_str(ctx, "fd3 call 1", "line_E\n", line);
	free(line);

	/* fd3 is now exhausted; fd1 and fd2 must still have their second lines */
	line = g_gnl(fd1);
	check_str(ctx, "fd1 call 2 (after fd3 EOF)", "line_B\n", line);
	free(line);

	line = g_gnl(fd2);
	check_str(ctx, "fd2 call 2 (after fd3 EOF)", "line_D\n", line);
	free(line);

	line = g_gnl(fd3);
	check_str(ctx, "fd3 call 2", NULL, line);
	free(line);

	line = g_gnl(fd1);
	check_str(ctx, "fd1 call 3", NULL, line);
	free(line);

	line = g_gnl(fd2);
	check_str(ctx, "fd2 call 3", NULL, line);
	free(line);

	close(fd1);
	close(fd2);
	close(fd3);
}
