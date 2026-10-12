#include "../../include/tester.h"
#include <string.h>

void	ft_bzero(void *s, size_t n);

void	test_ft_bzero(t_ctx *ctx)
{
	char	ref[16];
	char	got[16];

	memset(ref, 'A', 16); memset(got, 'A', 16);
	memset(ref, 0, 8); ft_bzero(got, 8);
	check_mem(ctx, "zero first 8 bytes (tail intact)", ref, got, 16);

	memset(ref, 0xFF, 16); memset(got, 0xFF, 16);
	memset(ref, 0, 16); ft_bzero(got, 16);
	check_mem(ctx, "zero all 16 bytes", ref, got, 16);

	memset(ref, 'B', 4); memset(got, 'B', 4);
	ft_bzero(got, 0);
	check_mem(ctx, "n=0 (no change)", ref, got, 4);

	char	single = 'X';
	ft_bzero(&single, 1);
	check_char(ctx, "single byte zeroed", '\0', single);

	/* guard bytes: zero the middle 4, edges must stay intact */
	{
		char	rbuf[10];
		char	gbuf[10];
		memset(rbuf, 0xFF, 10); memset(gbuf, 0xFF, 10);
		memset(rbuf + 3, 0, 4); ft_bzero(gbuf + 3, 4);
		check_mem(ctx, "zero middle 4 of 10, edges intact", rbuf, gbuf, 10);
	}
	/* stress: 8192 bytes */
	{
		static char	rbig[8192];
		static char	gbig[8192];
		memset(rbig, 0xAA, 8192); memset(gbig, 0xAA, 8192);
		memset(rbig, 0, 8192);    ft_bzero(gbig, 8192);
		check_mem(ctx, "bzero 8192 bytes (stress)", rbig, gbig, 8192);
	}
}
