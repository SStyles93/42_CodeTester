#include "tester.h"
#ifndef _WIN32
# include <signal.h>
# include <sys/mman.h>
# include <sys/wait.h>
#endif

int	g_test_timeout = 10;

#ifndef _WIN32

static void	report_child_status(t_ctx *ctx, const char *label, int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		ctx->fail++;
		sig = WTERMSIG(status);
		if (sig == SIGALRM)
			printf("    " RED "✗ TIMEOUT" RESET " %s did not finish within"
				" %ds (infinite loop?)\n", label, g_test_timeout);
		else
			printf("    " RED "✗ CRASH" RESET " %s: %s (signal %d)\n",
				label, strsignal(sig), sig);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		ctx->fail++;
		printf("    " RED "✗ EXIT %d" RESET " %s: memory error or leak reported"
			" by ASan/valgrind (see output above)\n", WEXITSTATUS(status),
			label);
	}
}

/*
** Run fn(ctx, arg) in a child process so a crash or infinite loop only takes
** down that piece of work (a whole suite, or one case inside a suite). The
** counters live in shared memory so the results recorded before a crash are
** kept. The child calls exit() (not _exit) so LeakSanitizer / valgrind check
** for leaks at the end of it.
*/
void	run_isolated(t_ctx *ctx, const char *label,
	void (*fn)(t_ctx *, const void *), const void *arg)
{
	t_ctx	*shared;
	pid_t	pid;
	int		status;

	shared = mmap(NULL, sizeof(t_ctx), PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (shared == MAP_FAILED)
	{
		fn(ctx, arg);
		return ;
	}
	*shared = *ctx;
	fflush(stdout);
	fflush(stderr);
	pid = fork();
	if (pid == 0)
	{
		setvbuf(stdout, NULL, _IONBF, 0);
		alarm(g_test_timeout);
		fn(shared, arg);
		exit(0);
	}
	if (pid < 0)
		fn(shared, arg);
	else
		waitpid(pid, &status, 0);
	*ctx = *shared;
	munmap(shared, sizeof(t_ctx));
	if (pid > 0)
		report_child_status(ctx, label, status);
}

#else

void	run_isolated(t_ctx *ctx, const char *label,
	void (*fn)(t_ctx *, const void *), const void *arg)
{
	(void)label;
	fn(ctx, arg);
}

#endif

static void	call_suite(t_ctx *ctx, const void *arg)
{
	((const t_suite *)arg)->run(ctx);
}

static void	run_one(t_suite *suite, t_ctx *ctx)
{
	int	total;

	if (ctx->verbose)
		printf(BOLD "\n── %s ──\n" RESET, suite->name);
	run_isolated(ctx, suite->name, call_suite, suite);
	total = ctx->pass + ctx->fail;
	if (ctx->fail == 0)
		printf("  " GREEN "✓" RESET " %-22s " GREEN "%d/%d" RESET "\n",
			suite->name, ctx->pass, total);
	else
		printf("  " RED "✗" RESET " %-22s " RED "%d/%d" RESET "\n",
			suite->name, ctx->pass, total);
}

void	run_suite(t_suite *suite, int verbose)
{
	t_ctx	ctx;

	ctx.pass = 0;
	ctx.fail = 0;
	ctx.verbose = verbose;
	run_one(suite, &ctx);
}

void	run_project_all(t_project *project, int verbose)
{
	t_ctx	ctx;
	int		i;
	int		total_pass;
	int		total_fail;

	total_pass = 0;
	total_fail = 0;
	printf(BOLD CYAN "\n=== %s ===" RESET "\n\n", project->name);

	i = 0;
	while (i < project->suite_count)
	{
		ctx.pass = 0;
		ctx.fail = 0;
		ctx.verbose = verbose;
		run_one(&project->suites[i], &ctx);
		total_pass += ctx.pass;
		total_fail += ctx.fail;
		i++;
	}
	printf("\n" BOLD "Summary: " RESET GREEN "%d passed" RESET
		", " RED "%d failed" RESET " / %d total\n",
		total_pass, total_fail, total_pass + total_fail);
}

void	run_suite_by_name(t_project *project, const char *name, int verbose)
{
	int	i;

	i = 0;
	while (i < project->suite_count)
	{
		if (strcmp(project->suites[i].name, name) == 0)
		{
			run_suite(&project->suites[i], verbose);
			return ;
		}
		i++;
	}
	fprintf(stderr, RED "Error:" RESET " no suite named '%s' in project '%s'\n",
		name, project->name);
}
