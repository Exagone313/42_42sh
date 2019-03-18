#include "twenty_one_sh.h"

void	put_in_foreground(t_job *job, int cont)
{
	my_tcsetpgrp(g_shell.term, job->pgid);
	if (cont)
	{
		tcsetattr(g_shell.term, TCSADRAIN, &job->tmodes);
		if (kill(-job->pgid, SIGCONT) < 0)
			ft_putstr_fd("kill (SIGCONT)\n", 2);
	}
	wait_for_job(job);
	my_tcsetpgrp(g_shell.term, g_shell.pgid);
	tcgetattr(g_shell.term, &job->tmodes);
	tcsetattr(g_shell.term, TCSADRAIN, &g_shell.cooked_tio);
}

void	put_in_background(t_job *job, int cont)
{
	if (cont)
		if (kill(-job->pgid, SIGCONT) < 0)
			ft_putstr_fd("kill (SIGCONT)", 2);
}