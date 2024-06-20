/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:18:05 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/06/18 23:17:08 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	child_process(char *argv, char **envp, t_fpids fp_id, int child_id)
{
	t_cmdargs	cmdargs;
	int			n;

	n = child_id;
	cmdargs = get_cmdargs(argv, envp);
	if (cmdargs.exit_status == 1)
	{
		close(fp_id.pipefd[n]);
		close(fp_id.pipefd[1 - n]);
		close(fp_id.file[n]);
		close(fp_id.file[1 - n]);
		exit(EXIT_FAILURE);
	}
	error_check(dup2(fp_id.file[n], n), "dup2", fp_id.file[n]);
	error_check(dup2(fp_id.pipefd[1 - n], 1 - n), "dup2", fp_id.pipefd[1 - n]);
	close(fp_id.pipefd[n]);
	close(fp_id.pipefd[1 - n]);
	close(fp_id.file[n]);
	close(fp_id.file[1 - n]);
	error_check(execve(cmdargs.cmd_path, cmdargs.cmd_argv, envp), "execve", 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_fpids	fp_id;

	init_check(argc, argv);
	fp_id.file[0] = open(argv[1], O_RDONLY);
	error_check(fp_id.file[0], "open", fp_id.file[0]);
	fp_id.file[1] = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	error_check(fp_id.file[1], "open", fp_id.file[1]);
	error_check(pipe(fp_id.pipefd), "pipe", 0);
	fp_id.pid[0] = fork();
	error_check(fp_id.pid[0], "fork", 0);
	if (fp_id.pid[0] == 0)
		child_process(argv[2], envp, fp_id, 0);
	fp_id.pid[1] = fork();
	error_check(fp_id.pid[1], "fork", 0);
	if (fp_id.pid[1] == 0)
		child_process(argv[3], envp, fp_id, 1);
	close(fp_id.pipefd[0]);
	close(fp_id.pipefd[1]);
	waitpid(fp_id.pid[0], NULL, 0);
	waitpid(fp_id.pid[1], NULL, 0);
	close(fp_id.file[0]);
	close(fp_id.file[1]);
	return (0);
}
