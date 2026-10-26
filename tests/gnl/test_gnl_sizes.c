#include "gnl_tester.h"
#ifdef _WIN32
# include <io.h>
#endif

/*
** Line lengths around BUFFER_SIZE ("both the buffer size and the line size
** can be of very different values"). Rebuild with another value to test it:
** make re BUFFER_SIZE=1 (or `make gnl-sizes` for 1, 42, 9999, 10000000).
*/

/* check_str, but long lines are shown as their length, not their bytes. */
static void	check_long_str(t_ctx *ctx, const char *label, const char *expected,
	const char *got)
{
	char	e[48];
	char	g[48];

	if (strlen(expected) <= 80)
	{
		check_str(ctx, label, expected, got);
		return ;
	}
	snprintf(e, sizeof(e), "%zu chars", strlen(expected));
	if (got)
		snprintf(g, sizeof(g), "%zu chars", strlen(got));
	else
		snprintf(g, sizeof(g), "NULL");
	if (got && strcmp(got, expected) == 0)
		report_pass(ctx, label, g);
	else
		report_fail(ctx, label, e, got && strlen(got) == strlen(expected)
			? "same length, different bytes" : g);
}

/* One line of `len` 'x' (+ "\n" if nl), read twice: the line, then NULL. */
static void	check_one_line(t_ctx *ctx, const char *name, size_t len, int nl)
{
	char	name_len[96];
	char	label[128];
	char	*content;
	char	*line;
	int		fd;

	content = gnl_repeat('x', len, nl ? "\n" : "");
	fd = content ? gnl_open_file(content) : -1;
	if (fd >= 0)
	{
		snprintf(name_len, sizeof(name_len), "%s (%zu chars%s)", name, len,
			nl ? " + \\n" : ", no \\n");
		line = g_gnl(fd);
		check_long_str(ctx, name_len, content, line);
		free(line);
		line = g_gnl(fd);
		snprintf(label, sizeof(label), "%s then NULL", name_len);
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
	check_one_line(ctx, "BUFFER_SIZE", BUFFER_SIZE, 0);
	check_one_line(ctx, "BUFFER_SIZE + 1", BUFFER_SIZE + 1, 0);
	check_one_line(ctx, "2 * BUFFER_SIZE", 2 * (size_t)BUFFER_SIZE, 1);
	/* "\n" exactly at the end of a read() (the line is BUFFER_SIZE bytes) */
	if (BUFFER_SIZE > 1)
		check_two_lines(ctx, "2 lines of BUFFER_SIZE bytes (incl. \\n)",
			BUFFER_SIZE - 1);
	check_two_lines(ctx, "2 lines of BUFFER_SIZE + 1 bytes (incl. \\n)",
		BUFFER_SIZE);
	check_one_line(ctx, "long line", 10000, 1);
	check_one_line(ctx, "long line", 10000, 0);
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
		check_ok(ctx, "1000-line file: every line in order", 1, NULL);
	line = g_gnl(fd);
	check_str(ctx, "1000-line file: NULL after the last line", NULL, line);
	free(line);
	gnl_drain(fd);
	close(fd);
}

/*
** The subject mandates that get_next_line() calls read() with count ==
** BUFFER_SIZE every time. We wrap read() (Linux, --wrap=read) and flag any
** call on the same fd where the count differs.
*/
void	test_gnl_read_count(t_ctx *ctx)
{
	char	why[96];
	char	*line;
	int		bad;
	int		fd;

	if (!read_check_available())
	{
		printf("    " YELLOW "!" RESET " read-count check needs the Linux"
			" read() wrapper (-Wl,--wrap) — skipped\n");
		return ;
	}
	fd = gnl_open_file("first line\nsecond line\nthird line\n");
	if (fd < 0)
		return ;
	read_check_begin(fd, BUFFER_SIZE);
	while ((line = g_gnl(fd)) != NULL)
		free(line);
	bad = read_check_end();
	snprintf(why, sizeof(why), "%d call%s used count != BUFFER_SIZE (%d)",
		bad, bad == 1 ? "" : "s", BUFFER_SIZE);
	check_ok(ctx, "read() is always called with count == BUFFER_SIZE",
		bad == 0, why);
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
	char	why[96];
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
	snprintf(why, sizeof(why), "read %ld bytes, at most %ld needed", offset,
		limit);
	check_ok(ctx, "does not read further than needed for the first line",
		offset >= 0 && offset <= limit, why);
	gnl_drain(fd);
	close(fd);
}
