/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:18:05 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/06/12 23:51:20 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5 || !argv[2] || !argv[3]) // Check for empty strings here?
	{
		ft_printf("execution failed. try: './pipex file1 cmd1 cmd2 file2'\n");
		return (1);
	}

	char **cmd1_argv;
	char **cmd2_argv;
	char *cmd1_path;
	char *cmd2_path;

	// work with CMD1 / CMD2 args in different processes

	cmd1_argv = ft_split(argv[2], ' ');
	// if (cmd1_argv == NULL || ft_isdigit(ft_atoi(cmd1_argv)))

	cmd1_path = get_cmdpath(envp, cmd1_argv);  // logically to perform check here

	if (cmd1_path == NULL)
	{
		ft_printf("command not found: %s\n", cmd1_argv[0]);
		free_split(cmd1_argv, NULL);
		exit(EXIT_FAILURE);
	}


	cmd2_argv = ft_split(argv[3], ' ');
	cmd2_path = get_cmdpath(envp, cmd2_argv);
	if (cmd2_path == NULL)
	{
		ft_printf("command not found: %s\n", cmd2_path[0]);
		free_split(cmd2_argv, NULL);
		exit(EXIT_FAILURE);
	}

	int infile;
	infile = open(argv[1], O_RDONLY);
	error_check(infile, "open", (void*)&infile);

	int pipefd[2];

	error_check(pipe(pipefd), "pipe", NULL);

	pid_t pid1;
	pid_t pid2;

	int outfile;

	pid1 = fork();
	error_check(pid1, "fork", NULL);
	if (pid1 == 0) // Child process 1. close reading side of pipe and redirect stdout to writing side of pipe
	{
		close(pipefd[0]);
		error_check(dup2(infile, STDIN_FILENO), "dup2", NULL);
		close(infile);
		error_check(dup2(pipefd[1], STDOUT_FILENO), "dup2", NULL);
		close(pipefd[1]);
		error_check(execve(cmd1_path, cmd1_argv, envp), "execve", NULL);
	}
	pid2 = fork();
	error_check(pid2, "fork", NULL);
	if (pid2 == 0) // Child process 2. close writing side of pipe and redirect STDIN to reading side of pipe
	{
		close(infile);
		close(pipefd[1]);
		error_check(dup2(pipefd[0], STDIN_FILENO), "dup2", NULL);
		close(pipefd[0]);
		outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		error_check(outfile, "open", (void*)&outfile);
		error_check(dup2(outfile, STDOUT_FILENO), "dup2", NULL);
		close(outfile);
		error_check(execve(cmd2_path, cmd2_argv, envp), "execve", NULL);
	}

	/* Close pipe ends in parent process
		so the cmd2 will not be eternally waiting
		for input as this side of pipe is open */

	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0); // Wait for Child 1
	close(infile);
	waitpid(pid2, NULL, 0); // Wait for Child 2
	free_split(cmd1_argv, cmd1_path);
	free_split(cmd2_argv, cmd2_path);
	return (0);
}
