/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 01:56:32 by avast             #+#    #+#             */
/*   Updated: 2023/02/17 17:45:36 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

int	*create_pipes(int nb_cmds)
{
	int	i;
	int	*pipes;

	pipes = malloc((nb_cmds - 1) * sizeof(int) * 2);
	if (!pipes)
		return (error_msg(MALLOC), free(pipes), NULL);
	i = 0;
	while (i < nb_cmds - 1)
	{
		if (pipe(pipes + (i * 2)) < 0)
			return (error_msg(PIPE), free(pipes), NULL);
		i++;
	}
	return (pipes);
}

int	redirect_fds(int i, t_pipex pipex)
{
	if (i == 0)
	{
		if (dup2(pipex.infile, 0) < 0 || dup2(pipex.pipes[1], 1) < 0)
			return (error_msg(DUP2), -1);
	}
	else if (i == pipex.nb_cmds - 1)
	{
		if (dup2(pipex.pipes[2 * i - 2], 0) < 0
			|| (pipex.outfile > 0 && dup2(pipex.outfile, 1) < 0))
			return (error_msg(DUP2), -1);
	}
	else
	{
		if (dup2(pipex.pipes[2 * i - 2], 0) < 0
			|| dup2(pipex.pipes[2 * i + 1], 1) < 0)
			return (error_msg(DUP2), -1);
	}
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
	close(pipex.infile);
	close(pipex.outfile);
	return (0);
}

int	execute_command(char *cmd, t_cmd **list, int i, t_pipex pipex)
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
		if (path)
			execve(path, arg, environ);
		exit_command(path, arg, list, pipex);
	}
	list_add_cmd(list, arg[0], path, pid);
	return (free_path(path, arg), 0);
}
