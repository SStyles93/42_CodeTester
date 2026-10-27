#include "tester.h"

/*
	Allocation tracker used by the "memory" suite.

	On Linux the tester is linked with -Wl,--wrap=malloc,--wrap=calloc,
	--wrap=free, so every call to those functions from the tester AND from
	libft.a is routed through the __wrap_* functions below. While tracking is
	on (mem_begin .. mem_end) we:
		- record every live block, so mem_end() can report what was never freed;
		- optionally make the Nth allocation return NULL, to check that libft
		  functions handle malloc failure (return NULL, free partial results).

	On other platforms (no GNU ld --wrap) the stubs at the bottom are used and
	mem_available() returns 0, so the memory suite is skipped.
*/

#ifdef MEMCHECK_WRAP

# define MEM_TABLE_SIZE 4096

void	*__real_malloc(size_t size);
void	*__real_calloc(size_t nmemb, size_t size);
void	__real_free(void *ptr);

typedef struct s_memstate
{
	int		tracking;
	int		fail_at;
	int		alloc_count;
	int		fail_hit;
	int		live_count;
	void	*live[MEM_TABLE_SIZE];
}	t_memstate;

static t_memstate	g_mem;

static void	mem_record(void *ptr)
{
	if (g_mem.live_count < MEM_TABLE_SIZE)
		g_mem.live[g_mem.live_count++] = ptr;
}

static void	mem_forget(void *ptr)
{
	int	i;

	i = 0;
	while (i < g_mem.live_count)
	{
		if (g_mem.live[i] == ptr)
		{
			g_mem.live[i] = g_mem.live[--g_mem.live_count];
			return ;
		}
		i++;
	}
}

/* Returns 1 if this allocation should be made to fail. */
static int	mem_should_fail(void)
{
	if (!g_mem.tracking)
		return (0);
	g_mem.alloc_count++;
	if (g_mem.fail_at && g_mem.alloc_count == g_mem.fail_at)
	{
		g_mem.fail_hit = 1;
		return (1);
	}
	return (0);
}

void	*__wrap_malloc(size_t size)
{
	void	*ptr;

	if (mem_should_fail())
		return (NULL);
	ptr = __real_malloc(size);
	if (g_mem.tracking && ptr)
		mem_record(ptr);
	return (ptr);
}

void	*__wrap_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (mem_should_fail())
		return (NULL);
	ptr = __real_calloc(nmemb, size);
	if (g_mem.tracking && ptr)
		mem_record(ptr);
	return (ptr);
}

void	__wrap_free(void *ptr)
{
	if (g_mem.tracking && ptr)
		mem_forget(ptr);
	__real_free(ptr);
}

int	mem_available(void)
{
	return (1);
}

void	mem_begin(int fail_at)
{
	g_mem.alloc_count = 0;
	g_mem.fail_hit = 0;
	g_mem.live_count = 0;
	g_mem.fail_at = fail_at;
	g_mem.tracking = 1;
}

int	mem_end(void)
{
	g_mem.tracking = 0;
	g_mem.fail_at = 0;
	return (g_mem.live_count);
}

int	mem_alloc_count(void)
{
	return (g_mem.alloc_count);
}

int	mem_fail_hit(void)
{
	return (g_mem.fail_hit);
}

#else

int	mem_available(void)
{
	return (0);
}

void	mem_begin(int fail_at)
{
	(void)fail_at;
}

int	mem_end(void)
{
	return (0);
}

int	mem_alloc_count(void)
{
	return (0);
}

int	mem_fail_hit(void)
{
	return (0);
}

#endif
