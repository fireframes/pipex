/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:18:05 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/06/07 22:02:24 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	int pipefd[2];
	pid_t cpid;
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read;
	char **where_arr;

	if (argc != 2)
	{
		return(-1);
	}
	char *argv_whereis[] = {"/usr/bin/whereis", "-b", argv[1], NULL};
	char *envp_whereis[] = {NULL};

	if (pipe(pipefd) == -1) // Create a pipe
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	cpid = fork(); // Fork program into 2 processes
	if (cpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (cpid == 0) // Child reads from pipe
	{
		close(pipefd[0]); // File descriptor for READING is not needed in this process and must be closed

		if (dup2(pipefd[1], STDOUT_FILENO) == -1) // We redefine stdout fd as pipe's WRITE fd, thus redirecting execve output
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if (execve("/usr/bin/whereis", argv_whereis, envp_whereis) == -1)
		{
			perror("execve");
			return (1);
		}

		close(pipefd[1]);

	}
	else
	{
		close(pipefd[1]); // File descriptor for WRITING is not needed in this process and must be closed

		bytes_read = read(pipefd[0], buffer, BUFFER_SIZE - 1);
		buffer[bytes_read] = '\0';

		where_arr = ft_split(buffer, ' ');

		wait(NULL); // Wait for Child process to finish;

		// char *argv_cmd1[] = {where_arr[1], NULL};
		// char *envp_cmd1[] = {NULL};

		// if (execve(where_arr[1], argv_cmd1, envp_cmd1) == -1)
		// {
		// 	perror("execve");
		// 	return (1);
		// }
		printf("Read from child end of pipe: %s\n", where_arr[1]);

		close(pipefd[0]);

		int i = 0;
		while (where_arr[i])
		{
			free(where_arr[i]);
			i++;
		}
		free(where_arr);
	}

	return (0);
}
