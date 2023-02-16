/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 01:56:32 by avast             #+#    #+#             */
/*   Updated: 2023/02/16 19:26:35 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

/*int	execute_last_command(char *full_cmd, int *status, int outfile)
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
		exit_last_command(path, arg);
	}
 	waitpid(pid, status, 0);
	*status = get_return_value(status, fdout); 
	if (path)
		free(path);
	return (free_tab(arg, -1), 0);
} */

int	execute_command(char *full_cmd, t_cmd **list)
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
	if (path)
		free(path);
	//*status = get_return_value(status, outfile);
	return (free_tab(arg, -1), 0);
}

/* int	get_command_status(char *path, char **arg)
{
	if (!path)
	{
		shell_error_msg(arg[0], NO_COMMAND);
		free_tab(arg, -1);
		return (NO_COMMAND);
	}
	if (access(path, X_OK) && access(path, F_OK) == 0)
	{
		shell_error_msg(arg[0], PERMISSION_DENIED);
		free_tab(arg, -1);
		free(path);
		return (PERMISSION_DENIED);
	}
	else
		return (1);
} */

int	get_return_value(int *status, int outfile)
{
	if (outfile < 0)
		return (1);
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	else
		return (0);
}

/* int	redirect_last_command(char *cmd, int *status, int outfile)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (error_msg(FORK));
	if (pid == 0)
		execute_command(cmd);
	get_return_value(status, outfile);
	return (0);
} */

int	redirect_last_command(char *cmd, int outfile, t_cmd **list)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (error_msg(FORK));
	if (pid == 0)
	{
		dup2(outfile, 1);
		close(outfile);
		execute_command(cmd, list);
	}
	return (0);
}

int	redirect_command(char *cmd, t_cmd **list)
{
	pid_t	pid;
	int		pfd[2];

	if (pipe(pfd) < 0)
		return (error_msg(PIPE));
	pid = fork();
	if (pid < 0)
		return (error_msg(FORK));
	if (pid == 0)
	{
		close(pfd[0]);
		dup2(pfd[1], 1);
		close(pfd[1]);
		execute_command(cmd, list);
		exit(0);
	}
	close(pfd[1]);
	dup2(pfd[0], 0);
	close(pfd[0]);
	return (0);
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
	//close(files[INFILE]);
	// regarder iciiiiii
	//redirect_last_command(argv[i], files[OUTFILE], list);
/* 	if (get_pid_list_size(list) >= 1)
	{
		dup2(files[OUTFILE], 1);
		close(files[OUTFILE]);
		execute_command(argv[i], list);
	} */
	dup2(files[OUTFILE], 1);
	execute_command(argv[i], list);
	return (close(files[OUTFILE]), 0);
}


/* int	pipex(int argc, char **argv, int files[2], int *status)
{
	int		i;
	int		pfd[2];
	int		fdin;
	int		fdout;

	if (!ft_strncmp(argv[1], "here_doc\0", 9))
		i = 3;
	else
		i = 2;
	fdin = files[INFILE];
	while (i < argc - 2)
	{
		if (pipe(pfd) == -1)
			return (error_msg(PIPE));
		if (i == argc - 2)
			fdout = files[OUTFILE];
		else
			fdout = pfd[1];
		execute_command(argv[i], fdin, fdout, status);
		fdin = pfd[0];
		i++;
	}
	execute_last_command(argv[i], fdin, files[OUTFILE], status);
	return (0);
} */

/* void	wait_all_pids(int forks)
{
	while (forks)
	{
		waitpid(-1, NULL, 0);
		forks--;
	}
}
 */