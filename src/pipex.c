/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:18:05 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/06/14 20:00:46 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5) // Check for empty strings here?
	{
		ft_printf("execution failed. try: './pipex file1 cmd1 cmd2 file2'\n");
		return (1);
	}
	if (!ft_strlen(argv[2]) || !ft_strlen(argv[3]))
	{
		if (!ft_strlen(argv[2]))
			ft_printf("command 1 was not provided\n");
		if (!ft_strlen(argv[3]))
			ft_printf("command 2 was not provided\n");
		return (2);
	}


	char **bin_path;
	char **cmd1_argv;
	char **cmd2_argv;
	char *cmd1_path;
	char *cmd2_path;
	
	bin_path = get_binpath(envp);
	if (!bin_path)
		return (3);

	cmd1_argv = ft_split(argv[2], ' ');
	if (!cmd1_argv)
		return (3);

	cmd1_path = get_cmdpath(cmd1_argv, bin_path);

	if (cmd1_path == NULL)
	{

		ft_printf("command not found: %s\n", cmd1_argv[0]);
		free_split(cmd1_argv);
		free_split(bin_path);
		return (4);
	}

	cmd2_argv = ft_split(argv[3], ' ');

	cmd2_path = get_cmdpath(cmd2_argv, bin_path);

	if (cmd2_path == NULL)
	{
		ft_printf("command not found: %s\n", cmd2_argv[0]);
		free(cmd1_path);
		free_split(cmd1_argv);
		free_split(cmd2_argv);
		free_split(bin_path);
		return (5);
	}

	int infile;
	int outfile;
	int pipefd[2];
	pid_t pid1;
	pid_t pid2;

	infile = open(argv[1], O_RDONLY);
	error_check(infile, "open", infile);
	error_check(pipe(pipefd), "pipe", 0);

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	error_check(outfile, "open", outfile);

	pid1 = fork();
	error_check(pid1, "fork", 0);
	if (pid1 == 0) // Child process 1.
	{
		close(pipefd[0]);
		error_check(dup2(infile, STDIN_FILENO), "dup2", 0);
		error_check(dup2(pipefd[1], STDOUT_FILENO), "dup2", 0);
		close(pipefd[1]);
		error_check(execve(cmd1_path, cmd1_argv, envp), "execve", 0);
	}
	
	pid2 = fork();
	error_check(pid2, "fork", 0);
	if (pid2 == 0) // Child process 2.
	{
		close(pipefd[1]);
		close(infile);
		error_check(dup2(pipefd[0], STDIN_FILENO), "dup2", 0);
		close(pipefd[0]);
		error_check(dup2(outfile, STDOUT_FILENO), "dup2", 0);
		error_check(execve(cmd2_path, cmd2_argv, envp), "execve", 0);
	}

	/* Close pipe ends in parent process
		so the cmd2 will not be eternally waiting
		for input as this side of pipe is open */
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0); // Wait for Child 1
	waitpid(pid2, NULL, 0); // Wait for Child 2
	close(infile);
	close(outfile);
	free(cmd1_path);
	free(cmd2_path);
	free_split(bin_path);
	free_split(cmd1_argv);
	free_split(cmd2_argv);
	return (0);
}
