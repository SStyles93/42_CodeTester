#include "../../include/tester.h"
#include <stdlib.h>
#include <string.h>

/*
** BUGS in ft_strjoin.c:
**   1. Calls ft_strlen() but the prototype is not visible — needs
**      #include "libft.h" (or <string.h> with strlen).
**   2. Missing #include <stdlib.h> for malloc.
*/

char	*ft_strjoin(char const *s1, char const *s2);

void	test_ft_strjoin(t_ctx *ctx)
{
	char	*r;

	/* basic join */
	r = ft_strjoin("Hello", " World");
	check_not_null(ctx, "strjoin basic non-NULL", r);
	check_str(ctx, "strjoin(\"Hello\",\" World\")", "Hello World", r);
	free(r);

	/* empty s1 */
	r = ft_strjoin("", "World");
	check_not_null(ctx, "strjoin(\"\",\"World\") non-NULL", r);
	check_str(ctx, "strjoin(\"\",\"World\")", "World", r);
	free(r);

	/* empty s2 */
	r = ft_strjoin("Hello", "");
	check_not_null(ctx, "strjoin(\"Hello\",\"\") non-NULL", r);
	check_str(ctx, "strjoin(\"Hello\",\"\")", "Hello", r);
	free(r);

	/* both empty */
	r = ft_strjoin("", "");
	check_not_null(ctx, "strjoin(\"\",\"\") non-NULL", r);
	check_str(ctx, "strjoin(\"\",\"\")", "", r);
	free(r);

	/* single chars */
	r = ft_strjoin("a", "b");
	check_not_null(ctx, "strjoin(\"a\",\"b\") non-NULL", r);
	check_str(ctx, "strjoin(\"a\",\"b\")", "ab", r);
	free(r);

	/* special characters */
	r = ft_strjoin("foo\n", "bar");
	check_not_null(ctx, "strjoin(newline) non-NULL", r);
	check_str(ctx, "strjoin(\"foo\\n\",\"bar\")", "foo\nbar", r);
	free(r);

	/* long strings */
	r = ft_strjoin("Hello, ", "World!");
	check_not_null(ctx, "strjoin(long) non-NULL", r);
	check_str(ctx, "strjoin(\"Hello, \",\"World!\")", "Hello, World!", r);
	free(r);

	/* result is a fresh allocation (different from both inputs) */
	{
		const char *s1 = "abc";
		const char *s2 = "def";
		r = ft_strjoin(s1, s2);
		check_not_null(ctx, "strjoin: new alloc non-NULL", r);
		if (r)
		{
			check_str(ctx, "strjoin: new alloc content", "abcdef", r);
			if ((void *)r != (void *)s1 && (void *)r != (void *)s2)
			{
				ctx->pass++;
				if (ctx->verbose)
					printf("    " GREEN "✓" RESET
						" strjoin: result is new allocation\n");
			}
			else
			{
				ctx->fail++;
				if (ctx->verbose)
					printf("    " RED "✗" RESET
						" strjoin: result reuses input pointer\n");
			}
			free(r);
		}
	}

	/* very long strings: 511 + 511 = 1022 chars */
	{
		static char	long1[512];
		static char	long2[512];
		memset(long1, 'a', 511); long1[511] = '\0';
		memset(long2, 'b', 511); long2[511] = '\0';
		r = ft_strjoin(long1, long2);
		check_not_null(ctx, "strjoin(511+'a', 511+'b') non-NULL", r);
		if (r)
		{
			check_size(ctx, "strjoin long: result length", 1022, strlen(r));
			free(r);
		}
	}
	/* NUL terminator at expected position */
	r = ft_strjoin("abc", "def");
	check_not_null(ctx, "strjoin NUL-term non-NULL", r);
	if (r)
	{
		check_char(ctx, "strjoin: NUL at pos 6", '\0', r[6]);
		free(r);
	}
	/* stress: 50 joins, all freeable */
	{
		int	i;
		int	ok;

		ok = 1;
		i = 0;
		while (i < 50)
		{
			char	*p = ft_strjoin("foo", "bar");
			if (!p || strcmp(p, "foobar") != 0) ok = 0;
			free(p);
			i++;
		}
		if (ok)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET
					" strjoin: 50 stress joins succeeded\n");
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET
					" strjoin: stress join failed\n");
		}
	}
}
