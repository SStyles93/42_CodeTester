#include "tester.h"
#include <sys/types.h>

/*
	read() wrapper that verifies every call from get_next_line() uses
	count == BUFFER_SIZE (Linux only, -Wl,--wrap=read).
	Only reads on the watched fd are checked; internal libc reads on
	other fds are ignored.
*/

#ifdef READCHECK_WRAP

ssize_t	__real_read(int fd, void *buf, size_t count);

typedef struct s_readstate
{
	int		tracking;
	int		watched_fd;
	size_t	expected;
	int		bad_count;
}	t_readstate;

static t_readstate	g_readcheck;

ssize_t	__wrap_read(int fd, void *buf, size_t count)
{
	if (g_readcheck.tracking && fd == g_readcheck.watched_fd
		&& count != g_readcheck.expected)
		g_readcheck.bad_count++;
	return (__real_read(fd, buf, count));
}

int	read_check_available(void)
{
	return (1);
}

void	read_check_begin(int fd, size_t expected)
{
	g_readcheck.bad_count = 0;
	g_readcheck.watched_fd = fd;
	g_readcheck.expected = expected;
	g_readcheck.tracking = 1;
}

int	read_check_end(void)
{
	g_readcheck.tracking = 0;
	return (g_readcheck.bad_count);
}

#else

int	read_check_available(void)
{
	return (0);
}

void	read_check_begin(int fd, size_t expected)
{
	(void)fd;
	(void)expected;
}

int	read_check_end(void)
{
	return (0);
}

#endif
