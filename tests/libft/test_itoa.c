#include "../../include/tester.h"
#include <stdlib.h>
#include <limits.h>

char	*ft_itoa(int n);

void	test_ft_itoa(t_ctx *ctx)
{
	char	*r;

	/* zero */
	r = ft_itoa(0);
	check_not_null(ctx, "itoa(0) non-NULL", r);
	check_str(ctx, "itoa(0)", "0", r);
	free(r);

	/* one */
	r = ft_itoa(1);
	check_not_null(ctx, "itoa(1) non-NULL", r);
	check_str(ctx, "itoa(1)", "1", r);
	free(r);

	/* small positive */
	r = ft_itoa(42);
	check_not_null(ctx, "itoa(42) non-NULL", r);
	check_str(ctx, "itoa(42)", "42", r);
	free(r);

	/* three digits */
	r = ft_itoa(100);
	check_not_null(ctx, "itoa(100) non-NULL", r);
	check_str(ctx, "itoa(100)", "100", r);
	free(r);

	/* -1 */
	r = ft_itoa(-1);
	check_not_null(ctx, "itoa(-1) non-NULL", r);
	check_str(ctx, "itoa(-1)", "-1", r);
	free(r);

	/* small negative */
	r = ft_itoa(-42);
	check_not_null(ctx, "itoa(-42) non-NULL", r);
	check_str(ctx, "itoa(-42)", "-42", r);
	free(r);

	/* INT_MAX */
	r = ft_itoa(2147483647);
	check_not_null(ctx, "itoa(INT_MAX) non-NULL", r);
	check_str(ctx, "itoa(2147483647)", "2147483647", r);
	free(r);

	/* INT_MIN — tricky: abs overflows if done naively */
	r = ft_itoa(-2147483648);
	check_not_null(ctx, "itoa(INT_MIN) non-NULL", r);
	check_str(ctx, "itoa(-2147483648)", "-2147483648", r);
	free(r);

	/* powers of ten */
	r = ft_itoa(10);
	check_not_null(ctx, "itoa(10) non-NULL", r);
	check_str(ctx, "itoa(10)", "10", r);
	free(r);

	r = ft_itoa(1000);
	check_not_null(ctx, "itoa(1000) non-NULL", r);
	check_str(ctx, "itoa(1000)", "1000", r);
	free(r);

	r = ft_itoa(-1000);
	check_not_null(ctx, "itoa(-1000) non-NULL", r);
	check_str(ctx, "itoa(-1000)", "-1000", r);
	free(r);

	/* single digit boundaries */
	r = ft_itoa(9);
	check_not_null(ctx, "itoa(9) non-NULL", r);
	if (r) { check_str(ctx, "itoa(9)", "9", r); free(r); }

	r = ft_itoa(-9);
	check_not_null(ctx, "itoa(-9) non-NULL", r);
	if (r) { check_str(ctx, "itoa(-9)", "-9", r); free(r); }

	/* two-digit boundaries */
	r = ft_itoa(99);
	check_not_null(ctx, "itoa(99) non-NULL", r);
	if (r) { check_str(ctx, "itoa(99)", "99", r); free(r); }

	r = ft_itoa(-99);
	check_not_null(ctx, "itoa(-99) non-NULL", r);
	if (r) { check_str(ctx, "itoa(-99)", "-99", r); free(r); }

	/* decade transitions */
	r = ft_itoa(-10);
	check_not_null(ctx, "itoa(-10) non-NULL", r);
	if (r) { check_str(ctx, "itoa(-10)", "-10", r); free(r); }

	r = ft_itoa(999);
	check_not_null(ctx, "itoa(999) non-NULL", r);
	if (r) { check_str(ctx, "itoa(999)", "999", r); free(r); }

	/* large number with alternating digits */
	r = ft_itoa(1234567890);
	check_not_null(ctx, "itoa(1234567890) non-NULL", r);
	if (r) { check_str(ctx, "itoa(1234567890)", "1234567890", r); free(r); }

	r = ft_itoa(-1234567890);
	check_not_null(ctx, "itoa(-1234567890) non-NULL", r);
	if (r) { check_str(ctx, "itoa(-1234567890)", "-1234567890", r); free(r); }
}
