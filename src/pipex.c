/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:18:05 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/06/12 00:37:41 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


// int main(int argc, char **argv, char **envp)
// {
// 	if (argc > 5)
// 	{
// 		printf("Program takes 3 arguments in form: './pipex file1 cmd1 cmd2 file2'\n");
// 		return (1);
// 	}
// 	char **cmd1_argv = ft_split(argv[1], ' ');
// 	char *cmd1_path = get_cmdpath(envp, cmd1_argv[0]);
	
// 	int infile = open("infile", O_RDONLY);
// 	if (infile == -1)
// 	{
// 		perror("open");
// 		close(infile);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (dup2(infile, STDIN_FILENO) == -1)
// 	{
// 		perror("dup2");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (execve(cmd1_path, cmd1_argv, envp) == -1)
// 	{
// 		perror("execve");
// 		exit(EXIT_FAILURE);
// 	}

// 	return (0);
// }


void free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int main(int argc, char **argv, char **envp)
{
	if (argc != 5)
	{
		printf("Program takes 4 arguments in form: './pipex file1 cmd1 cmd2 file2'\n");
		return (1);
	}

	int infile = open(argv[1], O_RDONLY);
	if (infile == -1)
	{
		perror("open");
		close(infile);
		exit(EXIT_FAILURE);
	}

	char **cmd1_argv = ft_split(argv[2], ' ');
	char **cmd2_argv = ft_split(argv[3], ' ');
	
	char *cmd1_path = get_cmdpath(envp, cmd1_argv[0]);
	char *cmd2_path = get_cmdpath(envp, cmd2_argv[0]);

	// char *cmd1_argv[] = {cmd1_path, NULL}; // MAIN PROBLEM!
	// char *cmd2_argv[] = {cmd2_path, NULL};

	int pipefd[2];
	
	if (pipe(pipefd) == -1) // Create a pipe
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid_t pid1;
	pid_t pid2;
	
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	
	if (pid1 == 0) // Child process 1. Here we close reading side of pipe and redirect stdout to writing side of pipe
	{
		close(pipefd[0]);

		if (dup2(infile, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		
		close(infile);

		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		
		close(pipefd[1]);  // and close write fd??
		
		if (execve(cmd1_path, cmd1_argv, envp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid2 == 0) // Child process 2. Here we close writing side of pipe and redirect STDIN to reading side of pipe
	{
		close(infile);
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);  // and close write fd??
		
		int outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (outfile == -1)
		{
			perror("open");
			close(outfile);
			exit(EXIT_FAILURE);
		}
		if (dup2(outfile, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(outfile);

		if (execve(cmd2_path, cmd2_argv, envp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	
	/* Close pipe ends in parent process 
		so the cmd2 will not be eternally waiting
		for input as this side of pipe is open */
		
	close(pipefd[0]); 
	close(pipefd[1]); 

	waitpid(pid1, NULL, 0); // Wait for Child 1
	close(infile);

	waitpid(pid2, NULL, 0); // Wait for Child 2
	
	free(cmd1_path);
	free(cmd2_path);
	free_split(cmd1_argv);
	free_split(cmd2_argv);
	
	return (0);
}
