/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 11:55:45 by avast             #+#    #+#             */
/*   Updated: 2023/01/26 15:35:21 by avast            ###   ########.fr       */
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
# include <string.h>

# define NO_COMMAND -2
# define NO_FILE -3
# define PERMISSION_DENIED -4

extern char	**environ;

typedef struct s_fork
{
	int		status;
	pid_t	pid;

}	t_fork;

typedef struct s_pipe
{
	int		pfd[2];
	int		a;
}	t_pipe;

int		check_path(void);
void	display_heredoc(int cmds);
void	error_msg(char *cmd, int type);
int		execute_command(char *full_cmd);
void	free_tab(char **tab, int index);
char	*ft_strjoin_cmd(char const *s1, char const *s2);
char	**get_command_arg(char *cmd);
char	*get_command_path(char *command);
int		get_infile_fd(int argc, char **argv);
int		get_outfile_fd(int argc, char **argv);
int		heredoc(char *limiter, int argc);
int		redirect_command(char *cmd);
int		pipex(int argc, char **argv, int infile, int outfile);
#endif