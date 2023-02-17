/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 11:55:45 by avast             #+#    #+#             */
/*   Updated: 2023/02/17 11:02:12 by avast            ###   ########.fr       */
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
# define EXECVE 127

extern char	**environ;

typedef struct s_cmd
{
	char			*name;
	char			*path;
	pid_t			pid;
	int				error;
	struct s_cmd	*next;
}			t_cmd;

int		check_environment(void);
int		check_path(void);
void	display_heredoc(int cmds);
int		error_msg(int type);
int		execute_command(char *path, char **arg);
int		execute_last_command(char *full_cmd, t_cmd **list);
//int		execute_command(char *full_cmd, int fdin, int fdout, int *status);
//int		execute_last_command(char *full_cmd, int fdin, int fdout, int *status);
int		exit_command(char *path, char **arg);
void	free_tab(char **tab, int index);
void	free_path(char *path, char **arg);
char	*ft_strjoin_cmd(char const *s1, char const *s2);
int		ft_strstr(const char *to_find, const char *big);
char	**get_command_arg(char *cmd);
char	*get_command_path(char *command);
int		get_command_status(char *path, char **arg);
int		get_infile_fd(int argc, char **argv);
int		get_outfile_fd(int argc, char **argv);
int		get_pid_list_size(t_cmd **list);
int		get_return_value(int *status, int outfile);
int		heredoc(char *limiter, int argc);
void	list_add_cmd(t_cmd **list, char *name, char *path, pid_t pid);
void	list_free_cmd(t_cmd **list);
int		pipex(int argc, char **argv, int files[2], t_cmd **list);
int		redirect_command(char *cmd, t_cmd **list);
int		redirect_last_command(char *cmd, int outfile, t_cmd **list);
//int		redirect_last_command(char *cmd, int *status, int outfile);
int		shell_error_msg(char *cmd, int type);
int		show_pids(t_cmd **list);
int		wait_all_pids(t_cmd **list, int oufile);

#endif