#include "../../include/tester.h"
#include "../../include/weak_functions.h"
#include <stdlib.h>
#include <string.h>

/*
** Memory suite: for every libft function that allocates, check that
**   1. a successful call leaves nothing behind once the result is freed;
**   2. when the Nth malloc inside the call fails (for every N the call
**      makes), the function returns NULL and frees everything it had
**      already allocated (e.g. ft_split's partial words).
** Needs the malloc wrapper from src/memcheck.c (Linux only).
*/

typedef struct s_memcase
{
	const char	*name;
	void		*(*call)(void);
	void		(*release)(void *result);
	const void	*fn;
}	t_memcase;

static char	map_upper(unsigned int i, char c)
{
	(void)i;
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

static void	*call_strdup(void)
{
	return (ft_strdup("hello"));
}

static void	*call_substr(void)
{
	return (ft_substr("Hello, World", 7, 5));
}

static void	*call_strjoin(void)
{
	return (ft_strjoin("foo", "bar"));
}

static void	*call_strtrim(void)
{
	return (ft_strtrim("  hello  ", " "));
}

static void	*call_split(void)
{
	return (ft_split("  one two  three four ", ' '));
}

static void	*call_itoa(void)
{
	return (ft_itoa(-2147483647 - 1));
}

static void	*call_strmapi(void)
{
	return (ft_strmapi("abc", map_upper));
}

static void	*call_calloc(void)
{
	return (ft_calloc(4, 8));
}

static void	*call_lstnew(void)
{
	return (ft_lstnew(NULL));
}

static void	release_plain(void *result)
{
	free(result);
}

static void	release_split(void *result)
{
	char	**arr;
	int		i;

	arr = (char **)result;
	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static void	check_case(t_ctx *ctx, const void *arg)
{
	const t_memcase	*mc;
	char			label[128];
	void			*result;
	int				allocs;
	int				leaked;
	int				n;

	mc = (const t_memcase *)arg;
	/* 1. normal call: result freed by us → nothing may remain */
	mem_begin(0);
	result = mc->call();
	allocs = mem_alloc_count();
	mc->release(result);
	leaked = mem_end();
	snprintf(label, sizeof(label), "%s: leaked blocks after free(result)",
		mc->name);
	check_int(ctx, label, 0, leaked);
	/* 2. make each of its allocations fail in turn */
	n = 1;
	while (n <= allocs)
	{
		mem_begin(n);
		result = mc->call();
		snprintf(label, sizeof(label), "%s: malloc #%d of %d fails → NULL",
			mc->name, n, allocs);
		check_ptr(ctx, label, NULL, result);
		mc->release(result);
		leaked = mem_end();
		snprintf(label, sizeof(label),
			"%s: malloc #%d of %d fails → leaked blocks", mc->name, n, allocs);
		check_int(ctx, label, 0, leaked);
		n++;
	}
}

void	test_memory(t_ctx *ctx)
{
	static const t_memcase	cases[] = {
		{"ft_strdup",  call_strdup,  release_plain, (const void *)ft_strdup},
		{"ft_substr",  call_substr,  release_plain, (const void *)ft_substr},
		{"ft_strjoin", call_strjoin, release_plain, (const void *)ft_strjoin},
		{"ft_strtrim", call_strtrim, release_plain, (const void *)ft_strtrim},
		{"ft_split",   call_split,   release_split, (const void *)ft_split},
		{"ft_itoa",    call_itoa,    release_plain, (const void *)ft_itoa},
		{"ft_strmapi", call_strmapi, release_plain, (const void *)ft_strmapi},
		{"ft_calloc",  call_calloc,  release_plain, (const void *)ft_calloc},
		{"ft_lstnew",  call_lstnew,  release_plain, (const void *)ft_lstnew},
	};
	size_t					i;

	if (!mem_available())
	{
		printf("    " YELLOW "!" RESET " memory checks need the Linux malloc"
			" wrapper (-Wl,--wrap) — skipped\n");
		return ;
	}
	i = 0;
	/* each function in its own process: one crash doesn't hide the rest */
	while (i < sizeof(cases) / sizeof(cases[0]))
	{
		if (!cases[i].fn)
			printf("    " YELLOW "⚠ MISSING" RESET " %s\n", cases[i].name);
		else
			run_isolated(ctx, cases[i].name, check_case, &cases[i]);
		i++;
	}
}
