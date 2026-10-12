#include "../../include/tester.h"
#include <stdlib.h>

char	*ft_strtrim(char const *s1, char const *set);

void	test_ft_strtrim(t_ctx *ctx)
{
	char	*r;

	/* trim both sides with single char */
	r = ft_strtrim("  hello  ", " ");
	check_not_null(ctx, "strtrim spaces non-NULL", r);
	check_str(ctx, "strtrim(\"  hello  \",\" \")", "hello", r);
	free(r);

	/* trim identical chars */
	r = ft_strtrim("aaabcaaa", "a");
	check_not_null(ctx, "strtrim 'a' non-NULL", r);
	check_str(ctx, "strtrim(\"aaabcaaa\",\"a\")", "bc", r);
	free(r);

	/* no trimming needed */
	r = ft_strtrim("hello", "x");
	check_not_null(ctx, "strtrim no-match non-NULL", r);
	check_str(ctx, "strtrim(\"hello\",\"x\")", "hello", r);
	free(r);

	/* empty input */
	r = ft_strtrim("", " ");
	check_not_null(ctx, "strtrim empty-input non-NULL", r);
	check_str(ctx, "strtrim(\"\",\" \")", "", r);
	free(r);

	/* empty set → no trimming */
	r = ft_strtrim("hello", "");
	check_not_null(ctx, "strtrim empty-set non-NULL", r);
	check_str(ctx, "strtrim(\"hello\",\"\")", "hello", r);
	free(r);

	/* entire string in set */
	r = ft_strtrim("   ", " ");
	check_not_null(ctx, "strtrim all-space non-NULL", r);
	check_str(ctx, "strtrim(\"   \",\" \")", "", r);
	free(r);

	/* multi-char set */
	r = ft_strtrim("xyhelloyx", "xy");
	check_not_null(ctx, "strtrim multi-set non-NULL", r);
	check_str(ctx, "strtrim(\"xyhelloyx\",\"xy\")", "hello", r);
	free(r);

	/* leading trim only */
	r = ft_strtrim("   hello", " ");
	check_not_null(ctx, "strtrim lead-only non-NULL", r);
	check_str(ctx, "strtrim(\"   hello\",\" \")", "hello", r);
	free(r);

	/* trailing trim only */
	r = ft_strtrim("hello   ", " ");
	check_not_null(ctx, "strtrim trail-only non-NULL", r);
	check_str(ctx, "strtrim(\"hello   \",\" \")", "hello", r);
	free(r);

	/* single char in set exactly equals string */
	r = ft_strtrim("a", "a");
	check_not_null(ctx, "strtrim single-match non-NULL", r);
	check_str(ctx, "strtrim(\"a\",\"a\")", "", r);
	free(r);

	/* set has chars not all in string */
	r = ft_strtrim("##hello##", "#!");
	check_not_null(ctx, "strtrim(\"##hello##\",\"#!\") non-NULL", r);
	check_str(ctx, "strtrim(\"##hello##\",\"#!\")", "hello", r);
	free(r);

	/* set with duplicate chars (should behave same as de-duped set) */
	r = ft_strtrim("  hello  ", "  ");
	check_not_null(ctx, "strtrim dup-set non-NULL", r);
	if (r) { check_str(ctx, "strtrim(\"  hello  \",\"  \")", "hello", r); free(r); }

	/* internal set chars must be preserved */
	r = ft_strtrim("abXYZba", "ab");
	check_not_null(ctx, "strtrim internal-preserved non-NULL", r);
	if (r) { check_str(ctx, "strtrim(\"abXYZba\",\"ab\")", "XYZ", r); free(r); }

	/* single char string not in set → returned unchanged */
	r = ft_strtrim("x", "y");
	check_not_null(ctx, "strtrim single-not-in-set non-NULL", r);
	if (r) { check_str(ctx, "strtrim(\"x\",\"y\")", "x", r); free(r); }

	/* mixed trim set order shouldn't matter */
	r = ft_strtrim("baahellobaa", "ab");
	check_not_null(ctx, "strtrim set-order non-NULL", r);
	if (r) { check_str(ctx, "strtrim(\"baahellobaa\",\"ab\")", "hello", r); free(r); }
}
