/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 00:14:08 by avast             #+#    #+#             */
/*   Updated: 2023/01/25 13:16:48 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

void	display_heredoc(int cmds)
{
	while (cmds--)
		ft_putstr_fd("pipe ", 1);
	ft_putstr_fd("heredoc> ", 1);
}

int	heredoc(char *limiter, int argc)
{
	t_pipe	p;
	char	*new_limiter;
	char	*line;
	char	*infile;

	p.a = pipe(p.pfd);
	if (p.a == -1)
		return (ft_putstr_fd("pipe failed\n", 2), -1);
	new_limiter = ft_strjoin(limiter, "\n");
	infile = 0;
	while (1)
	{
		display_heredoc(argc - 5);
		line = get_next_line(0);
		if (!ft_strncmp(line, new_limiter, ft_strlen(line)))
			break ;
		infile = ft_strjoin_gnl(infile, line);
		free(line);
	}
	ft_putstr_fd(infile, p.pfd[1]);
	return (close(p.pfd[1]), free(infile), free(line),
		free(new_limiter), p.pfd[0]);
}

int	get_infile_fd(int argc, char **argv)
{
	int	infile;

	if (!ft_strncmp(argv[1], "here_doc\0", 9))
		infile = heredoc(argv[2], argc);
	else
	{
		infile = open(argv[1], O_RDONLY);
		if (infile == -1 && access(argv[1], F_OK) == 0)
		{
			error_msg(argv[1], PERMISSION_DENIED);
			return (PERMISSION_DENIED);
		}
		else if (infile == -1 && access(argv[1], F_OK) == -1)
		{
			error_msg(argv[1], NO_FILE);
			return (NO_FILE);
		}
	}
	return (infile);
}

int	get_outfile_fd(int argc, char **argv)
{
	int	outfile;

	if (!ft_strncmp(argv[1], "here_doc\0", 9))
		outfile = open(argv[argc - 1], O_RDWR | O_CREAT | O_APPEND, 0644);
	else
		outfile = open(argv[argc - 1], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1 && access(argv[argc - 1], F_OK) == 0)
	{
		error_msg(argv[argc - 1], PERMISSION_DENIED);
		return (PERMISSION_DENIED);
	}
	else if (outfile == -1)
		return (ft_putstr_fd("file opening or creation failed: ", 2),
			ft_putstr_fd(argv[argc - 1], 2), ft_putstr_fd("\n", 2), -1);
	return (outfile);
}
