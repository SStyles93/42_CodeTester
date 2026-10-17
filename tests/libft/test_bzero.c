#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

void	test_ft_bzero(t_ctx *ctx)
{
	char	ref[10];
	char	got[10];

	/* zero the middle 4, edges must stay intact */
	memset(ref, 0xFF, 10); memset(got, 0xFF, 10);
	memset(ref + 3, 0, 4); ft_bzero(got + 3, 4);
	check_mem(ctx, "zero middle 4 of 10, edges intact", ref, got, 10);

	memset(ref, 'B', 10); memset(got, 'B', 10);
	ft_bzero(got, 0);
	check_mem(ctx, "n=0 (no change)", ref, got, 10);

	/* stress: 8192 bytes */
	{
		static char	rbig[8192];
		static char	gbig[8192];
		memset(rbig, 0xAA, 8192); memset(gbig, 0xAA, 8192);
		memset(rbig, 0, 8192);    ft_bzero(gbig, 8192);
		check_mem(ctx, "bzero 8192 bytes (stress)", rbig, gbig, 8192);
	}
}
