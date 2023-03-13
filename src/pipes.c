/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 01:56:32 by avast             #+#    #+#             */
/*   Updated: 2023/03/13 11:27:43 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

int	redirect_fds(int i, t_pipex pipex, char *path, char **arg)
{
	if (i == 1)
	{
		if ((dup2(pipex.infile, 0) < 0 && pipex.infile > 0)
			|| dup2(pipex.pipes[1], 1) < 0)
			return (free_path(path, arg), exit_and_free(DUP2, pipex));
		if (pipex.infile < 0)
			return (free_path(path, arg), exit_and_free(0, pipex));
	}
	else if (i == pipex.nb_cmds)
	{
		if (dup2(pipex.last_pipes[0], 0) < 0
			|| (pipex.outfile > 0 && dup2(pipex.outfile, 1) < 0))
			return (free_path(path, arg), exit_and_free(DUP2, pipex));
		if (pipex.outfile < 0)
			return (free_path(path, arg), exit_and_free(0, pipex));
	}
	else
	{
		if (dup2(pipex.last_pipes[0], 0) < 0 || dup2(pipex.pipes[1], 1) < 0)
			return (free_path(path, arg), exit_and_free(DUP2, pipex));
	}
	return (close_pipes(pipex), 0);
}

int	close_pipes(t_pipex pipex)
{
	if (pipex.pipes[0] > 0)
		close(pipex.pipes[0]);
	if (pipex.pipes[1] > 0)
		close(pipex.pipes[1]);
	if (pipex.last_pipes[0] > 0)
		close(pipex.last_pipes[0]);
	if (pipex.last_pipes[1] > 0)
		close(pipex.last_pipes[1]);
	if (pipex.infile >= 0)
	{
		if (close(pipex.infile) == -1)
			return (exit_and_free(CLOSE, pipex));
	}
	if (pipex.outfile >= 0)
	{
		if (close(pipex.outfile) == -1)
			return (exit_and_free(CLOSE, pipex));
	}
	return (0);
}

int	exit_command(char *path, char **arg, t_pipex pipex)
{
	if (path == 0 || access(path, F_OK) == -1)
	{
		ft_free_chartab(arg, -1);
		if (path)
			free(path);
		free_cmd(pipex.errors);
		exit (127);
	}
	else
	{
		ft_free_chartab(arg, -1);
		free(path);
		free_cmd(pipex.errors);
		exit (126);
	}
}

int	update_pipes(t_pipex *pipex, int i)
{
	if (pipex->last_pipes[0] > 0)
		close(pipex->last_pipes[0]);
	pipex->last_pipes[0] = pipex->pipes[0];
	pipex->last_pipes[1] = pipex->pipes[1];
	close(pipex->last_pipes[1]);
	pipex->pipes[0] = -1;
	pipex->pipes[0] = -1;
	if (i < pipex->nb_cmds - 1)
	{
		if (pipe(pipex->pipes) == -1)
			return (exit_and_free(PIPE, *pipex));
	}
	return (0);
}

int	execute_command(char *cmd, int i, t_pipex *pipex)
{
	pid_t	pid;
	char	**arg;
	char	*path;

	arg = ft_split(cmd, 32);
	if (!arg)
		return (exit_and_free(MALLOC, *pipex));
	path = get_command_path(arg[0], pipex->envp);
	pid = fork();
	if (pid < 0)
	{
		free_path(path, arg);
		return (exit_and_free(FORK, *pipex));
	}
	if (pid == 0)
	{
		redirect_fds(i, *pipex, path, arg);
		if (path)
			execve(path, arg, pipex->envp);
		exit_command(path, arg, *pipex);
	}
	list_add_cmd(pipex->errors, arg[0], path, pid);
	update_pipes(pipex, i);
	return (free_path(path, arg), 0);
}
