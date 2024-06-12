/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 23:14:41 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/06/12 22:41:55 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char **get_binpath(char **envp)
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

char *get_cmdpath(char **envp, char **cmd_argv)
{
	char	**bin_path;
	char	*tmp_path;
	char	*cmd_path;
	int		i;

	bin_path = get_binpath(envp);
	i = 0;
	while (bin_path[i])
	{
		tmp_path = ft_strjoin(bin_path[i], "/");
		cmd_path = ft_strjoin(tmp_path, cmd_argv[0]);
		free(tmp_path);
		if (!access(cmd_path, F_OK))
			break ;
		free(cmd_path);
		i++;
	}
	if (!bin_path[i])
	{
		free_split(bin_path, NULL);
		return (NULL);
	}
	free_split(bin_path, NULL);
	return (cmd_path);
}
