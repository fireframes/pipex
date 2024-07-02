/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 23:15:38 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/06/20 14:04:37 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include "ft_printf.h"
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>

typedef struct s_fpids
{
	int		file[2];
	int		pipefd[2];
	pid_t	pid[2];
}	t_fpids;

typedef struct s_cmdargs
{
	char	**bin_path;
	char	**cmd_argv;
	char	*cmd_path;
	int		exit_status;
}	t_cmdargs;

t_cmdargs	get_cmdargs(char *argv, char **envp);
char		**get_binpath(char **envp);
char		*get_cmdpath(char **cmd_argv, char **bin_path);
int			error_check(int ret_val, const char *err_msg, int close_fd);
void		free_split(char **split);
void		init_check(int argc, char **argv);
void		close_fpids(t_fpids fp_id, int n);

#endif
