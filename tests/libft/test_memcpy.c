#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

void	test_ft_memcpy(t_ctx *ctx)
{
	const char	src[] = "Hello, World!";
	char		ref[20];
	char		got[20];

	/* partial copy: bytes after n must stay intact */
	memset(ref, 'X', 20); memset(got, 'X', 20);
	memcpy(ref, src, 5); ft_memcpy(got, src, 5);
	check_mem(ctx, "copy 5 bytes, tail intact", ref, got, 20);

	memset(ref, 'X', 4); memset(got, 'X', 4);
	memcpy(ref, src, 0); ft_memcpy(got, src, 0);
	check_mem(ctx, "n=0 (no change)", ref, got, 4);

	char	dst[10] = {0};
	void	*ret = ft_memcpy(dst, src, 5);
	check_ptr(ctx, "returns dst pointer", (void *)dst, ret);

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
