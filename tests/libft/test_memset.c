#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

void	test_ft_memset(t_ctx *ctx)
{
	char	ref[32];
	char	got[32];

	memset(ref, 'A', 10); ft_memset(got, 'A', 10);
	check_mem(ctx, "fill 10 with 'A'", ref, got, 10);

	memset(ref, 'X', 4); memset(got, 'X', 4);
	memset(ref, 0, 0); ft_memset(got, 0, 0);
	check_mem(ctx, "n=0 (no change)", ref, got, 4);

	char	buf[4] = {0};
	void	*ret = ft_memset(buf, 1, 4);
	check_ptr(ctx, "returns dst pointer", (void *)buf, ret);

	/* value > 127: int c is truncated to unsigned char */
	{
		char	rbuf2[4];
		char	gbuf2[4];
		memset(rbuf2, 200, 4);
		ft_memset(gbuf2, 200, 4);
		check_mem(ctx, "fill 4 with 200 (high byte cast)", rbuf2, gbuf2, 4);
	}
	/* guard bytes: only first n bytes change, tail is intact */
	{
		char	rbuf3[8];
		char	gbuf3[8];
		memset(rbuf3, 0xFF, 8); memset(gbuf3, 0xFF, 8);
		memset(rbuf3, 0, 4);    ft_memset(gbuf3, 0, 4);
		check_mem(ctx, "guard bytes: only first 4 zeroed", rbuf3, gbuf3, 8);
	}
	/* stress: large buffer (4096 bytes) */
	{
		static char	rbig[4096];
		static char	gbig[4096];
		memset(rbig, 'Z', 4096);
		ft_memset(gbig, 'Z', 4096);
		check_mem(ctx, "fill 4096 with 'Z' (stress)", rbig, gbig, 4096);
	}
}
