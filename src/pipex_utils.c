/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:34:32 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/06/12 22:40:06 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **split, char *cmd_path)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	if (cmd_path)
		free(cmd_path);
}

int	error_check(int ret_val, const char *err_msg, void *close_fd)
{
	if (ret_val == -1)
	{
		perror(err_msg);
		if (*(int*)close_fd)
			close(*(int*)close_fd);
		// free_split(bin_path, NULL); // Use struct to free everything
		// free_split(cmd_argv, NULL);
		exit(EXIT_FAILURE);
	}
	return (0);
}
