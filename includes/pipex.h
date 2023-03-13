/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 11:55:45 by avast             #+#    #+#             */
/*   Updated: 2023/03/13 11:31:52 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>

# define INFILE 1
# define OUTFILE 0
# define NO_ERROR 0
# define NO_COMMAND -2
# define NO_FILE -3
# define PERMISSION_DENIED -4
# define ENV -5
# define ARG -6
# define PIPE -7
# define FORK -8
# define MALLOC -9
# define FILE_CREATION -10
# define CLOSE -11
# define DUP2 -12

typedef struct s_cmd
{
	char			*name;
	char			*path;
	pid_t			pid;
	int				error;
	struct s_cmd	*next;
}			t_cmd;

typedef struct s_pipex
{
	int		argc;
	char	**argv;
	char	**envp;
	int		nb_cmds;
	int		infile;
	int		outfile;
	int		pipes[2];
	int		last_pipes[2];
	int		first_cmd;
	t_cmd	**errors;
}	t_pipex;

int		check_environment(char **envp);
int		check_path(char **envp);
int		close_pipes(t_pipex pipex);
void	display_heredoc(int cmds);
int		error_msg(int type);
int		execute_command(char *cmd, int i, t_pipex *pipex);
int		exit_and_free(int type, t_pipex pipex);
int		exit_command(char *path, char **arg, t_pipex pipex);
void	free_path(char *path, char **arg);
char	*ft_strjoin_cmd(char const *s1, char const *s2);
char	**get_command_arg(char *cmd);
char	*get_command_path(char *command, char **envp);
int		get_infile_fd(int argc, char **argv, char **envp);
int		get_outfile_fd(int argc, char **argv);
int		get_pipex_infos(int argc, char **argv, t_pipex *pipex, t_cmd **list);
int		get_return_value(int *status, int outfile);
int		heredoc(char *limiter, int argc);
int		list_add_cmd(t_cmd **list, char *name, char *path, pid_t pid);
int		list_create_elem(t_cmd *new, char *name, char *path, pid_t pid);
void	free_cmd(t_cmd **list);
int		redirect_fds(int i, t_pipex pipex, char *path, char **arg);
int		shell_error_msg(char *cmd, int type, char **envp);
int		update_pipes(t_pipex *pipex, int i);
int		wait_all_pids(t_cmd **list, t_pipex pipex);

#endif