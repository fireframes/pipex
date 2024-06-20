/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmdargs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 23:14:41 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/06/18 23:17:44 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_binpath(char **envp)
{
	char	**bin_path;
	char	*envpath;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp("PATH=", envp[i], 5))
			break ;
		i++;
	}
	if (envp[i] == NULL)
	{
		ft_printf("PATH environment variable not defined or muted.\n");
		return (NULL);
	}
	envpath = envp[i] + 5;
	bin_path = ft_split(envpath, ':');
	if (bin_path == NULL)
		return (NULL);
	return (bin_path);
}

char	*get_cmdpath(char **cmd_argv, char **bin_path)
{
	char	*tmp_path;
	char	*cmd_path;
	int		i;

	i = 0;
	while (bin_path[i])
	{
		tmp_path = ft_strjoin(bin_path[i], "/");
		cmd_path = ft_strjoin(tmp_path, cmd_argv[0]);
		free(tmp_path);
		if (!access(cmd_path, F_OK | X_OK))
			return (cmd_path);
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (NULL);
}

t_cmdargs	get_cmdargs(char *argv, char **envp)
{
	t_cmdargs	cmdargs;

	cmdargs.exit_status = 0;
	cmdargs.bin_path = get_binpath(envp);
	if (cmdargs.bin_path == NULL)
		exit(EXIT_FAILURE);
	cmdargs.cmd_argv = ft_split(argv, ' ');
	if (cmdargs.cmd_argv == NULL)
	{
		free_split(cmdargs.bin_path);
		exit(EXIT_FAILURE);
	}
	cmdargs.cmd_path = get_cmdpath(cmdargs.cmd_argv, cmdargs.bin_path);
	if (cmdargs.cmd_path == NULL)
	{
		ft_printf("command not found: %s\n", cmdargs.cmd_argv[0]);
		free_split(cmdargs.cmd_argv);
		free_split(cmdargs.bin_path);
		cmdargs.exit_status = 1;
	}
	return (cmdargs);
}
