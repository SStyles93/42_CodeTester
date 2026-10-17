#include "pf_tester.h"

/* Plain text, empty formats and several conversions in one call. */

static void	long_format(t_ctx *ctx)
{
	static char	fmt[20001];
	int			i;

	i = 0;
	while (i < 20000)
	{
		fmt[i] = 'a' + (i % 26);
		if (i % 1000 == 999)
			fmt[i] = '\n';
		i++;
	}
	fmt[i] = '\0';
	PF_L(ctx, "20000-char format, no conversion", fmt);
	memcpy(fmt + 5000, "%d", 2);
	memcpy(fmt + 10000, "%s", 2);
	memcpy(fmt + 15000, "%x", 2);
	PF_L(ctx, "20000-char format with %d %s %x inside", fmt, -42, "mid",
		0xBEEFu);
}

void	test_ft_printf(t_ctx *ctx)
{
	/* plain text */
	PF(ctx, "");
	PF(ctx, "a");
	PF(ctx, "hello");
	PF(ctx, "hello world");
	PF(ctx, "\n");
	PF(ctx, "\n\n\n");
	PF(ctx, "tab\there");
	PF(ctx, "line1\nline2\n");
	PF(ctx, " ");
	PF(ctx, "   leading and trailing   ");
	PF(ctx, "!\"#$&'()*+,-./:;<=>?@[\\]^_`{|}~");
	PF(ctx, "0123456789");
	PF(ctx, "\x01\x02\x7f");
	PF(ctx, "\xc3\xa9t\xc3\xa9 utf-8");
	PF(ctx, "\r\v\f\a\b");

	/* one conversion surrounded by text */
	PF(ctx, "[%d]", 42);
	PF(ctx, "before %s after", "middle");
	PF(ctx, "%c at start", 'X');
	PF(ctx, "at end %c", 'Y');
	PF(ctx, "%d", 0);
	PF(ctx, "%s", "");
	PF(ctx, "%s%s", "", "");
	PF(ctx, "%s%s%s", "a", "b", "c");

	/* every mandatory conversion in a single call */
	PF(ctx, "%c%s%p%d%i%u%x%X%%", 'A', "str", (void *)0x1234, -1, 2,
		3u, 255u, 255u);
	PF(ctx, "c=%c s=%s p=%p d=%d i=%i u=%u x=%x X=%X %%",
		'z', "hello", (void *)ctx, INT_MIN, INT_MAX, UINT_MAX, 0xabcdefu,
		0xabcdefu);
	PF(ctx, "%d %s %d %s %d", 1, "two", 3, "four", 5);
	PF(ctx, "%s|%c|%d|%x", "x", 'y', 'z', 'z');
	PF(ctx, "%%%c%%%s%%%d%%", 'a', "b", 1);

	/* many arguments: va_list must advance correctly */
	PF(ctx, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20);
	PF(ctx, "%c%c%c%c%c%c%c%c%c%c%c%c", 'h', 'e', 'l', 'l', 'o', ' ', 'w',
		'o', 'r', 'l', 'd', '!');
	PF(ctx, "%s %s %s %s %s %s %s %s", "a", "bb", "ccc", "dddd", "", "f",
		"gg", "hhh");
	PF(ctx, "%p %p %p %p", (void *)1, (void *)2, (void *)0xff,
		(void *)0xffff);
	PF(ctx, "%x %X %u %d %i %c %s %p", 1u, 2u, 3u, 4, 5, '6', "7",
		(void *)8);

	/* arguments of different sizes interleaved (pointer is 8 bytes) */
	PF(ctx, "%p %d %p %c %p %s", (void *)-1, -1, (void *)0, 'q',
		(void *)0x42, "end");

	long_format(ctx);
}
	