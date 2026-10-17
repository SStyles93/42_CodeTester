#include "pf_tester.h"

void	test_pf_c(t_ctx *ctx)
{
	int	c;

	PF(ctx, "%c", 'a');
	PF(ctx, "%c", 'Z');
	PF(ctx, "%c", '0');
	PF(ctx, "%c", ' ');
	PF(ctx, "%c", '%');
	PF(ctx, "%c", '\n');
	PF(ctx, "%c", '\t');
	PF(ctx, "%c", '~');

	/* NUL is written and counted */
	PF(ctx, "%c", '\0');
	PF(ctx, "%c", 0);
	PF(ctx, "a%cb", '\0');
	PF(ctx, "%c%c%c", '\0', '\0', '\0');
	PF(ctx, "%s%c%s", "left", 0, "right");

	/* int argument converted to unsigned char */
	PF(ctx, "%c", 127);
	PF(ctx, "%c", 128);
	PF(ctx, "%c", 200);
	PF(ctx, "%c", 255);
	PF(ctx, "%c", -1);
	PF(ctx, "%c", -128);
	PF(ctx, "%c", 256 + 'A');
	PF(ctx, "%c", 300);

	PF(ctx, "%c%c", 'o', 'k');
	PF(ctx, "%c %c %c", '1', '2', '3');
	PF(ctx, "[%c]", 'x');

	/* every byte value */
	c = 0;
	while (c < 256)
	{
		char	label[32];

		snprintf(label, sizeof(label), "\"%%c\", %d", c);
		PF_L(ctx, label, "%c", c);
		c++;
	}
}
