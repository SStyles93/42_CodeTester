#include "../../include/tester.h"
#include <string.h>

void	*ft_memcpy(void *dest, const void *src, size_t n);

void	test_ft_memcpy(t_ctx *ctx)
{
	const char	src[] = "Hello, World!";
	char		ref[20];
	char		got[20];

	memset(ref, 0, 20); memset(got, 0, 20);
	memcpy(ref, src, 5); ft_memcpy(got, src, 5);
	check_mem(ctx, "copy 5 bytes", ref, got, 5);

	memcpy(ref, src, strlen(src) + 1); ft_memcpy(got, src, strlen(src) + 1);
	check_mem(ctx, "copy full string + NUL", ref, got, strlen(src) + 1);

	memset(ref, 'X', 4); memset(got, 'X', 4);
	memcpy(ref, src, 0); ft_memcpy(got, src, 0);
	check_mem(ctx, "n=0 (no change)", ref, got, 4);

	char	dst[10] = {0};
	void	*ret = ft_memcpy(dst, src, 5);
	check_ptr(ctx, "returns dst pointer", (void *)dst, ret);

	/* single byte copy */
	{
		char	rsingle = 'Y';
		char	gsingle = 'Y';
		char	src2 = 'X';
		memcpy(&rsingle, &src2, 1);
		ft_memcpy(&gsingle, &src2, 1);
		check_char(ctx, "copy 1 byte", rsingle, gsingle);
	}
	/* binary data with embedded zeros */
	{
		const unsigned char	bsrc[] = {0x00, 0x01, 0xFF, 0x7F, 0x80, 0x42};
		unsigned char		rbuf[6];
		unsigned char		gbuf[6];
		memcpy(rbuf, bsrc, 6);
		ft_memcpy(gbuf, bsrc, 6);
		check_mem(ctx, "copy binary with embedded zero", rbuf, gbuf, 6);
	}
	/* stress: 1024-byte copy */
	{
		static char	src3[1024];
		static char	rbig[1024];
		static char	gbig[1024];
		memset(src3, 'P', 1024);
		memcpy(rbig, src3, 1024);
		ft_memcpy(gbig, src3, 1024);
		check_mem(ctx, "copy 1024 bytes (stress)", rbig, gbig, 1024);
	}
}
