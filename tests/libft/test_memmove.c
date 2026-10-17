#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

void	test_ft_memmove(t_ctx *ctx)
{
	char	ref[24];
	char	got[24];

	memset(ref, 0, 24); memset(got, 0, 24);
	strcpy(ref, "Hello"); strcpy(got, "Hello");
	memmove(ref + 10, ref, 6); ft_memmove(got + 10, got, 6);
	check_mem(ctx, "non-overlapping copy", ref + 10, got + 10, 6);

	strcpy(ref, "Hello"); strcpy(got, "Hello");
	memmove(ref + 2, ref, 4); ft_memmove(got + 2, got, 4);
	check_mem(ctx, "overlap: dst > src (shift right)", ref, got, 8);

	strcpy(ref, "  Hello"); strcpy(got, "  Hello");
	memmove(ref, ref + 2, 6); ft_memmove(got, got + 2, 6);
	check_mem(ctx, "overlap: dst < src (shift left)", ref, got, 7);

	strcpy(ref, "Test"); strcpy(got, "Test");
	memmove(ref + 2, ref, 0); ft_memmove(got + 2, got, 0);
	check_mem(ctx, "n=0 (no change)", ref, got, 4);

	char	buf[10] = "ABCDE";
	void	*ret = ft_memmove(buf + 2, buf, 3);
	check_ptr(ctx, "returns dest pointer", (void *)(buf + 2), ret);

	/* src == dst: must be a no-op */
	{
		char	same[8] = "ABCDEFG";
		char	ref2[8];
		memcpy(ref2, same, 8);
		ft_memmove(same, same, 4);
		check_mem(ctx, "memmove src==dst (no change)", ref2, same, 8);
	}
	/* large overlapping copy (512 bytes, shift by 1) */
	{
		static char	rbig[513];
		static char	gbig[513];
		memset(rbig, 'Q', 512); rbig[512] = '\0';
		memset(gbig, 'Q', 512); gbig[512] = '\0';
		memmove(rbig + 1, rbig, 511);
		ft_memmove(gbig + 1, gbig, 511);
		check_mem(ctx, "memmove 511-byte rightward shift (stress)", rbig, gbig, 512);
	}
}
