/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 01:56:32 by avast             #+#    #+#             */
/*   Updated: 2023/02/17 16:20:03 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

int	execute_command(char *path, char **arg)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (error_msg(FORK));
	else if (pid == 0)
	{
		execve(path, arg, environ);
		exit_command(path, arg);
	}
	return (0);
}

int	execute_last_command(char *full_cmd, t_cmd **list)
{
	char	**arg;
	char	*path;
	pid_t	pid;

	arg = ft_split(full_cmd, 32);
	if (!arg)
		return (error_msg(MALLOC));
	path = get_command_path(arg[0]);
	pid = fork();
	if (pid == -1)
		return (error_msg(FORK));
	else if (pid == 0)
	{
		execve(path, arg, environ);
		exit_command(path, arg);
	}
	list_add_cmd(list, arg[0], path, pid);
	free_path(path, arg);
	return (0);
}

int	redirect_fds(int i, t_pipex pipex)
{
	if (i == 0)
		(dup2(pipex.infile, 0), dup2(pipex.pipes[1], 1));
	else if (i == pipex.nb_cmds - 1)
		(dup2(pipex.pipes[2 * i - 2], 0), dup2(pipex.outfile, 1));
	else
		(dup2(pipex.pipes[2 * i - 2], 0), dup2(pipex.pipes[2 * i + 1], 1));
	close_pipes(pipex);
	return (0);
}

int	close_pipes(t_pipex pipex)
{
	int	i;

	i = 0;
	while (i < pipex.nb_cmds - 1)
	{
		close(pipex.pipes[i * 2]);
		close(pipex.pipes[i * 2 + 1]);
		i++;
	}
	return (0);
}

int	redirect_command(char *cmd, t_cmd **list, int i, t_pipex pipex)
{
	pid_t	pid;
	char	**arg;
	char	*path;

	arg = ft_split(cmd, 32);
	if (!arg)
		return (error_msg(MALLOC));
	path = get_command_path(arg[0]);
	pid = fork();
	if (pid < 0)
		return (error_msg(FORK));
	if (pid == 0)
	{
		redirect_fds(i, pipex);
		execve(path, arg, environ);
		exit_command(path, arg);
	}
	list_add_cmd(list, arg[0], path, pid);
	return (free_path(path, arg), 0);
}

int	f_pipex(int argc, char **argv, t_pipex pipex, t_cmd **list)
{
	int		i;

	i = pipex.first_cmd;
/* 	dup2(pipex.infile, 0);
	close(pipex.infile); */
	while (i < argc - 1)
	{
		//ft_printf("i = %d\n", i - pipex.first_cmd);
		redirect_command(argv[i], list, i - pipex.first_cmd, pipex);
		i++;
	}
	close_pipes(pipex);
	return (0);
}
