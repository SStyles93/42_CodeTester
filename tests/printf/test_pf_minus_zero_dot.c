#include "pf_tester.h"

/* Bonus part 1: '-', '0', '.' and minimum field width. */

static void	width_only(t_ctx *ctx)
{
	PF(ctx, "%1d", 42);
	PF(ctx, "%2d", 42);
	PF(ctx, "%5d", 42);
	PF(ctx, "%5d", -42);
	PF(ctx, "%12d", INT_MIN);
	PF(ctx, "%11d", INT_MIN);
	PF(ctx, "%10d", INT_MIN);
	PF(ctx, "%5i", 0);
	PF(ctx, "%5u", 42u);
	PF(ctx, "%12u", UINT_MAX);
	PF(ctx, "%8x", 0xbeefu);
	PF(ctx, "%8X", 0xbeefu);
	PF(ctx, "%5s", "abc");
	PF(ctx, "%2s", "abcdef");
	PF(ctx, "%5s", "");
	PF(ctx, "%10s", (char *)NULL);
	PF(ctx, "%3c", 'a');
	PF(ctx, "%1c", 'a');
	PF(ctx, "%3c", '\0');
	PF(ctx, "%20p", (void *)0xdeadbeef);
	PF(ctx, "%2p", (void *)0xdeadbeef);
	PF(ctx, "%10p", NULL);
	PF(ctx, "%200d", 1);
	PF(ctx, "%5000s", "big");
	PF(ctx, "%100000c", 'w');
}

static void	minus(t_ctx *ctx)
{
	PF(ctx, "%-5d|", 42);
	PF(ctx, "%-5d|", -42);
	PF(ctx, "%-1d|", 42);
	PF(ctx, "%-12d|", INT_MIN);
	PF(ctx, "%-5i|", 0);
	PF(ctx, "%-5u|", 42u);
	PF(ctx, "%-12u|", UINT_MAX);
	PF(ctx, "%-8x|", 0xbeefu);
	PF(ctx, "%-8X|", 0xbeefu);
	PF(ctx, "%-5s|", "abc");
	PF(ctx, "%-2s|", "abcdef");
	PF(ctx, "%-5s|", "");
	PF(ctx, "%-10s|", (char *)NULL);
	PF(ctx, "%-3c|", 'a');
	PF(ctx, "%-3c|", '\0');
	PF(ctx, "%-20p|", (void *)0xdeadbeef);
	PF(ctx, "%-10p|", NULL);
	PF(ctx, "%---5d|", 7);
	PF(ctx, "%-d|", 7);
	PF(ctx, "|%-4c|%-4s|%-4d|%-4x|", 'a', "bc", 12, 0xfu);
}

static void	zero(t_ctx *ctx)
{
	PF(ctx, "%05d", 42);
	PF(ctx, "%05d", -42);
	PF(ctx, "%05d", 0);
	PF(ctx, "%01d", 42);
	PF(ctx, "%02d", -1);
	PF(ctx, "%012d", INT_MIN);
	PF(ctx, "%011d", INT_MIN);
	PF(ctx, "%015d", INT_MAX);
	PF(ctx, "%05i", -7);
	PF(ctx, "%05u", 42u);
	PF(ctx, "%015u", UINT_MAX);
	PF(ctx, "%08x", 0xbeefu);
	PF(ctx, "%08X", 0xbeefu);
	PF(ctx, "%08x", 0u);
	PF(ctx, "%0d", 5);
	PF(ctx, "%00005d", 5);
	/* '-' wins over '0' */
	PF(ctx, "%-05d|", 42);
	PF(ctx, "%0-5d|", -42);
	PF(ctx, "%-08x|", 0xffu);
	PF(ctx, "%0-8u|", 3u);
	/* a precision disables '0' for integers */
	PF(ctx, "%08.3d", 42);
	PF(ctx, "%08.3d", -42);
	PF(ctx, "%08.0d", 0);
	PF(ctx, "%08.d", 0);
	PF(ctx, "%08.10d", 42);
	PF(ctx, "%08.3x", 0xfu);
	PF(ctx, "%08.3u", 7u);
}

static void	precision(t_ctx *ctx)
{
	PF(ctx, "%.0d", 0);
	PF(ctx, "%.d", 0);
	PF(ctx, "%.0d", 1);
	PF(ctx, "%.0i", -1);
	PF(ctx, "%.1d", 0);
	PF(ctx, "%.3d", 7);
	PF(ctx, "%.3d", -7);
	PF(ctx, "%.3d", 12345);
	PF(ctx, "%.5d", -42);
	PF(ctx, "%.10d", INT_MIN);
	PF(ctx, "%.11d", INT_MIN);
	PF(ctx, "%.12d", INT_MAX);
	PF(ctx, "%.0u", 0u);
	PF(ctx, "%.5u", 42u);
	PF(ctx, "%.12u", UINT_MAX);
	PF(ctx, "%.0x", 0u);
	PF(ctx, "%.0X", 0u);
	PF(ctx, "%.4x", 0xfu);
	PF(ctx, "%.4X", 0xfu);
	PF(ctx, "%.10x", UINT_MAX);
	PF(ctx, "%.100d", 1);
	PF(ctx, "%.1000d", -1);
	/* width + precision */
	PF(ctx, "%8.3d", 42);
	PF(ctx, "%8.3d", -42);
	PF(ctx, "%-8.3d|", 42);
	PF(ctx, "%-8.3d|", -42);
	PF(ctx, "%3.8d", 42);
	PF(ctx, "%5.0d", 0);
	PF(ctx, "%-5.0d|", 0);
	PF(ctx, "%5.d", 0);
	PF(ctx, "%5.0u", 0u);
	PF(ctx, "%5.0x", 0u);
	PF(ctx, "%10.5x", 0xabcu);
	PF(ctx, "%-10.5X|", 0xabcu);
	/* strings */
	PF(ctx, "%.0s", "hello");
	PF(ctx, "%.s", "hello");
	PF(ctx, "%.1s", "hello");
	PF(ctx, "%.3s", "hello");
	PF(ctx, "%.5s", "hello");
	PF(ctx, "%.10s", "hello");
	PF(ctx, "%.3s", "");
	PF(ctx, "%10.3s|", "hello");
	PF(ctx, "%-10.3s|", "hello");
	PF(ctx, "%2.4s|", "hello");
	PF(ctx, "%.2s%.3s", "abc", "defgh");
	/* NULL string with precision (glibc: "" when precision < 6) */
	PF(ctx, "%.0s", (char *)NULL);
	PF(ctx, "%.1s", (char *)NULL);
	PF(ctx, "%.5s", (char *)NULL);
	PF(ctx, "%.6s", (char *)NULL);
	PF(ctx, "%.7s", (char *)NULL);
	PF(ctx, "%10.3s|", (char *)NULL);
	PF(ctx, "%-10.6s|", (char *)NULL);
}

void	test_pf_minus_zero_dot(t_ctx *ctx)
{
	width_only(ctx);
	minus(ctx);
	zero(ctx);
	precision(ctx);
}
