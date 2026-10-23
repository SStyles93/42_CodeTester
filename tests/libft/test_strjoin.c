#include "../../include/tester.h"
#include <stdlib.h>
#include <string.h>
#include "../../include/weak_functions.h"

void	test_ft_strjoin(t_ctx *ctx)
{
	char	*r;

	/* basic join */
	r = ft_strjoin("Hello", " World");
	check_str(ctx, "strjoin(\"Hello\",\" World\")", "Hello World", r);
	free(r);

	/* empty s1 */
	r = ft_strjoin("", "World");
	check_str(ctx, "strjoin(\"\",\"World\")", "World", r);
	free(r);

	/* empty s2 */
	r = ft_strjoin("Hello", "");
	check_str(ctx, "strjoin(\"Hello\",\"\")", "Hello", r);
	free(r);

	/* both empty */
	r = ft_strjoin("", "");
	check_str(ctx, "strjoin(\"\",\"\")", "", r);
	free(r);

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
}
