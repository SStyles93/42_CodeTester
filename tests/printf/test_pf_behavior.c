#include "pf_tester.h"
#ifndef _WIN32
# include <fcntl.h>
# include <signal.h>
#endif

/*
** Behaviour that is not about formatting:
**   - output goes straight to fd 1, in order with raw write() calls
**     (no buffering inside ft_printf);
**   - return value is -1 when write() fails, and a failure does not leak
**     into the next call;
**   - no state carried over between calls;
**   - no leaks (and correct behaviour if malloc fails), Linux only.
*/

/* Run ft_printf with fd 1 replaced by fd (or closed when fd < 0). */
#define PF_ON_FD(ret, fd, ...) do { \
	int	pf_saved_; \
	fflush(stdout); \
	pf_saved_ = dup(STDOUT_FILENO); \
	if ((fd) < 0) \
		close(STDOUT_FILENO); \
	else \
		dup2((fd), STDOUT_FILENO); \
	(ret) = ft_printf(__VA_ARGS__); \
	dup2(pf_saved_, STDOUT_FILENO); \
	close(pf_saved_); \
} while (0)

static void	unbuffered(t_ctx *ctx)
{
	t_pfcap	cap;
	int		len;

	if (pf_capture_begin(&cap) != 0)
	{
		ctx->fail++;
		return ;
	}
	write(STDOUT_FILENO, "A", 1);
	ft_printf("B%dC", 1);
	write(STDOUT_FILENO, "D", 1);
	ft_printf("%s", "E");
	ft_printf("");
	write(STDOUT_FILENO, "F", 1);
	ft_printf("%c%%", 'G');
	len = pf_capture_end(&cap, g_pf_got, PF_BUF_SIZE);
	check_int(ctx, "interleaved write()/ft_printf: length", 9, len);
	check_str(ctx, "interleaved write()/ft_printf: order", "AB1CDEFG%",
		g_pf_got);
}

static void	no_state_between_calls(t_ctx *ctx)
{
	PF(ctx, "%s", "long string first");
	PF(ctx, "%s", "x");
	PF(ctx, "%d", INT_MIN);
	PF(ctx, "%d", 1);
	PF(ctx, "%X", 0xABCu);
	PF(ctx, "%x", 0xabcu);
	PF(ctx, "%p", NULL);
	PF(ctx, "%p", (void *)1);
	PF(ctx, "%c", '\0');
	PF(ctx, "%c", 'a');
}

static void	return_values(t_ctx *ctx)
{
	static char	big[100001];
	t_pfcap		cap;
	int			r;

	if (pf_capture_begin(&cap) != 0)
	{
		ctx->fail++;
		return ;
	}
	r = ft_printf("");
	pf_capture_end(&cap, g_pf_got, PF_BUF_SIZE);
	check_int(ctx, "ft_printf(\"\") returns 0", 0, r);
	pf_capture_begin(&cap);
	r = ft_printf("%c", '\0');
	pf_capture_end(&cap, g_pf_got, PF_BUF_SIZE);
	check_int(ctx, "ft_printf(\"%c\", 0) returns 1", 1, r);
	pf_capture_begin(&cap);
	r = ft_printf("%s", "");
	pf_capture_end(&cap, g_pf_got, PF_BUF_SIZE);
	check_int(ctx, "ft_printf(\"%s\", \"\") returns 0", 0, r);
	memset(big, 'z', sizeof(big) - 1);
	pf_capture_begin(&cap);
	r = ft_printf("%s", big);
	pf_capture_end(&cap, g_pf_got, PF_BUF_SIZE);
	check_int(ctx, "ft_printf(\"%s\", <100000 z>) returns 100000", 100000, r);
}

#ifndef _WIN32

static void	write_errors(t_ctx *ctx)
{
	int		r;
	int		fd;
	int		pfd[2];
	char	path[] = "/tmp/pf_tester_XXXXXX";

	PF_ON_FD(r, -1, "abc");
	check_int(ctx, "stdout closed: \"abc\" returns -1", -1, r);
	PF_ON_FD(r, -1, "%d", 42);
	check_int(ctx, "stdout closed: \"%d\" returns -1", -1, r);
	PF_ON_FD(r, -1, "%s", "str");
	check_int(ctx, "stdout closed: \"%s\" returns -1", -1, r);
	PF_ON_FD(r, -1, "%c", 'c');
	check_int(ctx, "stdout closed: \"%c\" returns -1", -1, r);
	PF_ON_FD(r, -1, "%u", 1u);
	check_int(ctx, "stdout closed: \"%u\" returns -1", -1, r);
	PF_ON_FD(r, -1, "%x", 1u);
	check_int(ctx, "stdout closed: \"%x\" returns -1", -1, r);
	PF_ON_FD(r, -1, "%p", (void *)ctx);
	check_int(ctx, "stdout closed: \"%p\" returns -1", -1, r);
	PF_ON_FD(r, -1, "%%");
	check_int(ctx, "stdout closed: \"%%\" returns -1", -1, r);

	/* stdout open read-only */
	fd = mkstemp(path);
	if (fd >= 0)
	{
		close(fd);
		fd = open(path, O_RDONLY);
		unlink(path);
		PF_ON_FD(r, fd, "abc%d", 1);
		check_int(ctx, "stdout read-only: returns -1", -1, r);
		close(fd);
	}
	else
		ctx->fail++;

	/* pipe with no reader (EPIPE) */
	if (pipe(pfd) == 0)
	{
		signal(SIGPIPE, SIG_IGN);
		close(pfd[0]);
		PF_ON_FD(r, pfd[1], "broken %s", "pipe");
		check_int(ctx, "stdout is a broken pipe: returns -1", -1, r);
		close(pfd[1]);
		signal(SIGPIPE, SIG_DFL);
	}
	else
		ctx->fail++;

# ifdef __linux__
	/* device full (ENOSPC) */
	fd = open("/dev/full", O_WRONLY);
	if (fd >= 0)
	{
		PF_ON_FD(r, fd, "full %d", 1);
		check_int(ctx, "stdout is /dev/full: returns -1", -1, r);
		close(fd);
	}
# endif

	/* a failed call must not affect the next one */
	PF_ON_FD(r, -1, "fail");
	PF(ctx, "works again %d", 42);
	PF(ctx, "");
	PF(ctx, "%s", "and again");
}

#endif

/* The same call as in memory(), with the fail_at-th allocation failing. */
static int	pf_failing_call(int fail_at, int *leaks)
{
	t_pfcap	cap;
	int		r;

	*leaks = 0;
	if (pf_capture_begin(&cap) != 0)
		return (-2);
	mem_begin(fail_at);
	r = ft_printf("%c|%s|%p|%d|%i|%u|%x|%X|%%|%s|%p",
			'c', "str", (void *)&cap, INT_MIN, INT_MAX, UINT_MAX, 0xabcu,
			0xabcu, NULL, NULL);
	*leaks = mem_end();
	pf_capture_end(&cap, g_pf_got, PF_BUF_SIZE);
	return (r);
}

static void	memory(t_ctx *ctx)
{
	t_pfcap	cap;
	int		leaks;
	int		allocs;
	int		expected;
	int		i;
	int		r;
	char	label[64];

	if (!mem_available())
	{
		if (ctx->verbose)
			printf("    " YELLOW "-" RESET " memory checks skipped"
				" (Linux only)\n");
		return ;
	}
	if (pf_capture_begin(&cap) != 0)
	{
		ctx->fail++;
		return ;
	}
	mem_begin(0);
	expected = ft_printf("%c|%s|%p|%d|%i|%u|%x|%X|%%|%s|%p",
			'c', "str", (void *)&cap, INT_MIN, INT_MAX, UINT_MAX, 0xabcu,
			0xabcu, NULL, NULL);
	ft_printf("%s", (char *)NULL);
	ft_printf("%c%c", '\0', 'x');
	allocs = mem_alloc_count();
	leaks = mem_end();
	pf_capture_end(&cap, g_pf_got, PF_BUF_SIZE);
	check_int(ctx, "no leaked allocation", 0, leaks);
	if (ctx->verbose)
		printf(DIM "        (ft_printf made %d allocation(s))" RESET "\n",
			allocs);
	i = 1;
	while (i <= allocs)
	{
		r = pf_failing_call(i, &leaks);
		snprintf(label, sizeof(label), "malloc #%d fails: returns -1 or ok", i);
		check_bool(ctx, label, 1, r == -1 || r == expected);
		snprintf(label, sizeof(label), "malloc #%d fails: no leak", i);
		check_int(ctx, label, 0, leaks);
		i++;
	}
}

void	test_pf_behavior(t_ctx *ctx)
{
	unbuffered(ctx);
	return_values(ctx);
	no_state_between_calls(ctx);
#ifndef _WIN32
	write_errors(ctx);
#endif
	memory(ctx);
}
