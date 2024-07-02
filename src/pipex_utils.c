/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:34:32 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/06/20 14:02:13 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	error_check(int ret_val, const char *err_msg, int close_fd)
{
	if (ret_val == -1)
	{
		perror(err_msg);
		if (close_fd > 0)
			close(close_fd);
		exit(EXIT_FAILURE);
	}
	return (0);
}

void	free_split(char **split)
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

void	init_check(int argc, char **argv)
{
	if (argc != 5)
	{
		ft_printf("execution failed. try: './pipex file1 cmd1 cmd2 file2'\n");
		exit(EXIT_FAILURE);
	}
	if (!ft_strlen(argv[2]) || !ft_strlen(argv[3]))
	{
		if (!ft_strlen(argv[2]))
			ft_printf("cmd1 was not provided\n");
		if (!ft_strlen(argv[3]))
			ft_printf("cmd2 was not provided\n");
		exit(EXIT_FAILURE);
	}
}

void	close_fpids(t_fpids fp_id, int n)
{
	close(fp_id.pipefd[n]);
	close(fp_id.pipefd[1 - n]);
	close(fp_id.file[n]);
	close(fp_id.file[1 - n]);
}
