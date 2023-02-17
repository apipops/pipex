/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 01:56:32 by avast             #+#    #+#             */
/*   Updated: 2023/02/17 13:05:04 by avast            ###   ########.fr       */
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

int	redirect_command(char *cmd, t_cmd **list)
{
	pid_t	pid;
	int		pfd[2];
	char	**arg;
	char	*path;

	arg = ft_split(cmd, 32);
	if (!arg)
		return (error_msg(MALLOC));
	path = get_command_path(arg[0]);
	if (pipe(pfd) < 0)
		return (error_msg(PIPE));
	pid = fork();
	if (pid < 0)
		return (error_msg(FORK));
	if (pid == 0)
	{
		(close(pfd[0]), dup2(pfd[1], 1), close(pfd[1]));
		execve(path, arg, environ);
		exit_command(path, arg);
		//execute_command(path, arg);
		//exit(0);
	}
	(close(pfd[1]), dup2(pfd[0], 0), close(pfd[0]));
	list_add_cmd(list, arg[0], path, pid);
	return (free_path(path, arg), 0);
}

int	pipex(int argc, char **argv, int files[2], t_cmd **list)
{
	int		i;

	if (!ft_strncmp(argv[1], "here_doc\0", 9))
		i = 3;
	else
		i = 2;
	dup2(files[INFILE], 0);
	close(files[INFILE]);
	while (i < argc - 2)
	{
		redirect_command(argv[i], list);
		i++;
	}
	dup2(files[OUTFILE], 1);
	execute_last_command(argv[i], list);
	//close les pipes
	return (close(files[OUTFILE]), 0);
}
