#include "gnl_tester.h"

/*
** "All heap-allocated memory must be properly freed": once get_next_line()
** has returned NULL at EOF and every line was freed, nothing may remain
** (the static variable included). When the Nth malloc fails, the call must
** return NULL without crashing, and reading on until EOF must not leak.
** Needs the malloc wrapper from src/memcheck.c (Linux only).
*/

#define GNL_MEM_CONTENT "first line\nsecond line\n\nlast line, no newline"

typedef struct s_gnlmem
{
	const char	*name;
	int			(*open)(const char *content);
}	t_gnlmem;

/* Read everything; returns the number of lines that came back. */
static int	read_all(int fd)
{
	char	*line;
	int		count;

	count = 0;
	while ((line = g_gnl(fd)) != NULL)
	{
		free(line);
		count++;
	}
	return (count);
}

static void	check_source(t_ctx *ctx, const void *arg)
{
	const t_gnlmem	*src;
	char			label[128];
	char			*line;
	int				allocs;
	int				fd;
	int				n;

	src = (const t_gnlmem *)arg;
	fd = src->open(GNL_MEM_CONTENT);
	if (fd < 0)
		return ;
	mem_begin(0);
	read_all(fd);
	allocs = mem_alloc_count();
	snprintf(label, sizeof(label), "%s: leaked blocks after reading to EOF",
		src->name);
	check_int(ctx, label, 0, mem_end());
	close(fd);
	n = 1;
	while (n <= allocs)
	{
		fd = src->open(GNL_MEM_CONTENT);
		if (fd < 0)
			return ;
		mem_begin(n);
		/* read until the call during which malloc #n failed */
		while (1)
		{
			line = g_gnl(fd);
			if (mem_fail_hit() || !line)
				break ;
			free(line);
		}
		snprintf(label, sizeof(label), "%s: malloc #%d of %d fails → NULL",
			src->name, n, allocs);
		check_ptr(ctx, label, NULL, line);
		free(line);
		read_all(fd);
		snprintf(label, sizeof(label),
			"%s: malloc #%d of %d fails → leaked blocks", src->name, n, allocs);
		check_int(ctx, label, 0, mem_end());
		close(fd);
		n++;
	}
}

void	test_gnl_memory(t_ctx *ctx)
{
	static const t_gnlmem	sources[] = {
		{"pipe", gnl_open_pipe},
		{"file", gnl_open_file},
	};
	size_t					i;

	if (!mem_available())
	{
		printf("    " YELLOW "!" RESET " memory checks need the Linux malloc"
			" wrapper (-Wl,--wrap) — skipped\n");
		return ;
	}
	i = 0;
	while (i < sizeof(sources) / sizeof(sources[0]))
	{
		run_isolated(ctx, sources[i].name, check_source, &sources[i]);
		i++;
	}
}
