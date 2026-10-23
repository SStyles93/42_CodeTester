#include "gnl_tester.h"
#ifdef _WIN32
# include <io.h>
#endif

/*
** Line lengths around BUFFER_SIZE ("both the buffer size and the line size
** can be of very different values"). Rebuild with another value to test it:
** make re BUFFER_SIZE=1 (or `make gnl-sizes` for 1, 42, 9999, 10000000).
*/

/* check_str, without printing lines too long to read. */
static void	check_long_str(t_ctx *ctx, const char *label, const char *expected,
	const char *got)
{
	if (strlen(expected) <= 80)
	{
		check_str(ctx, label, expected, got);
		return ;
	}
	check_bool(ctx, label, 1, got && strcmp(got, expected) == 0);
	if (ctx->verbose && (!got || strcmp(got, expected) != 0))
		printf(DIM "        expected %zu chars, got %s%zu chars" RESET "\n",
			strlen(expected), got ? "" : "NULL, ", got ? strlen(got) : 0);
}

/* One line of `len` 'x' (+ "\n" if nl), read twice: the line, then NULL. */
static void	check_one_line(t_ctx *ctx, const char *name, size_t len, int nl)
{
	char	label[96];
	char	*content;
	char	*line;
	int		fd;

	content = gnl_repeat('x', len, nl ? "\n" : "");
	fd = content ? gnl_open_file(content) : -1;
	if (fd >= 0)
	{
		line = g_gnl(fd);
		snprintf(label, sizeof(label), "%s (%zu chars%s)", name, len,
			nl ? " + \\n" : ", no \\n");
		check_long_str(ctx, label, content, line);
		free(line);
		line = g_gnl(fd);
		snprintf(label, sizeof(label), "%s then NULL", name);
		check_str(ctx, label, NULL, line);
		free(line);
		gnl_drain(fd);
		close(fd);
	}
	free(content);
}

/* Two lines of `len` chars: the second must not start with bytes of the first. */
static void	check_two_lines(t_ctx *ctx, const char *name, size_t len)
{
	char	label[96];
	char	*first;
	char	*second;
	char	*content;
	char	*line;
	int		fd;

	first = gnl_repeat('a', len, "\n");
	second = gnl_repeat('b', len, "\n");
	content = (first && second) ? malloc(2 * (len + 1) + 1) : NULL;
	fd = -1;
	if (content)
	{
		strcpy(content, first);
		strcat(content, second);
		fd = gnl_open_file(content);
	}
	if (fd >= 0)
	{
		line = g_gnl(fd);
		snprintf(label, sizeof(label), "%s: line 1", name);
		check_long_str(ctx, label, first, line);
		free(line);
		line = g_gnl(fd);
		snprintf(label, sizeof(label), "%s: line 2", name);
		check_long_str(ctx, label, second, line);
		free(line);
		gnl_drain(fd);
		close(fd);
	}
	free(first);
	free(second);
	free(content);
}

void	test_gnl_sizes(t_ctx *ctx)
{
	if (BUFFER_SIZE > 1)
		check_one_line(ctx, "BUFFER_SIZE - 1", BUFFER_SIZE - 1, 1);
	check_one_line(ctx, "BUFFER_SIZE", BUFFER_SIZE, 1);
	check_one_line(ctx, "BUFFER_SIZE + 1", BUFFER_SIZE + 1, 1);
	check_one_line(ctx, "BUFFER_SIZE, no \\n", BUFFER_SIZE, 0);
	check_one_line(ctx, "BUFFER_SIZE + 1, no \\n", BUFFER_SIZE + 1, 0);
	check_one_line(ctx, "2 * BUFFER_SIZE", 2 * (size_t)BUFFER_SIZE, 1);
	/* "\n" exactly at the end of a read() (the line is BUFFER_SIZE bytes) */
	if (BUFFER_SIZE > 1)
		check_two_lines(ctx, "2 lines of BUFFER_SIZE", BUFFER_SIZE - 1);
	check_two_lines(ctx, "2 lines of BUFFER_SIZE + 1", BUFFER_SIZE);
	check_one_line(ctx, "10000-char line", 10000, 1);
	check_one_line(ctx, "10000-char line", 10000, 0);
}

/*
** A file of 1000 numbered lines (bigger than a pipe can hold): every line in
** order, then NULL.
*/
void	test_gnl_big_file(t_ctx *ctx)
{
	char	expected[32];
	char	label[64];
	char	*content;
	char	*line;
	size_t	pos;
	int		fd;
	int		i;
	int		ok;

	content = malloc(1000 * 32);
	if (!content)
		return ;
	pos = 0;
	i = 0;
	while (i < 1000)
		pos += sprintf(content + pos, "line %d of the big file\n", i++);
	fd = gnl_open_file(content);
	free(content);
	if (fd < 0)
		return ;
	ok = 1;
	i = 0;
	while (i < 1000 && ok)
	{
		sprintf(expected, "line %d of the big file\n", i);
		line = g_gnl(fd);
		if (!line || strcmp(line, expected) != 0)
		{
			snprintf(label, sizeof(label), "1000-line file: line %d", i);
			check_str(ctx, label, expected, line);
			ok = 0;
		}
		free(line);
		i++;
	}
	if (ok)
		check_bool(ctx, "1000-line file: every line in order", 1, 1);
	line = g_gnl(fd);
	check_str(ctx, "1000-line file: NULL after the last line", NULL, line);
	free(line);
	gnl_drain(fd);
	close(fd);
}

/*
** "Try to read as little as possible each time get_next_line() is called...
** Don't read the whole file and then process each line."
** After the first call, the file offset must not be past the read() that
** found the first "\n".
*/
void	test_gnl_read_little(t_ctx *ctx)
{
	char	*content;
	char	*line;
	long	offset;
	long	limit;
	size_t	first_len;
	int		fd;

	content = gnl_repeat('x', (size_t)BUFFER_SIZE + 5000, "\n");
	if (!content)
		return ;
	memcpy(content, "first line\n", 11);
	first_len = 11;
	fd = gnl_open_file(content);
	free(content);
	if (fd < 0)
		return ;
	line = g_gnl(fd);
	check_str(ctx, "first line returned", "first line\n", line);
	free(line);
	offset = (long)lseek(fd, 0, SEEK_CUR);
	limit = (long)((first_len + BUFFER_SIZE - 1) / BUFFER_SIZE) * BUFFER_SIZE;
	check_bool(ctx, "does not read further than needed for the first line",
		1, offset >= 0 && offset <= limit);
	if (ctx->verbose && offset > limit)
		printf(DIM "        read %ld bytes, at most %ld needed" RESET "\n",
			offset, limit);
	gnl_drain(fd);
	close(fd);
}
