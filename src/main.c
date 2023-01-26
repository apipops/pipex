/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 11:55:15 by avast             #+#    #+#             */
/*   Updated: 2023/01/26 02:16:14 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

int	pipex(char *cmd)
{
	pid_t	pid;
	int		status;
	int		pfd[2];

	if (pipe(pfd) < 0)
		return (ft_putstr_fd("piped failed\n", 2), -1);
	pid = fork();
	if (pid < 0)
		return (ft_putstr_fd("fork failed\n", 2), -1);
	if (pid == 0)
	{
		close(pfd[0]);
		dup2(pfd[1], 1);
		close(pfd[1]);
		execute_command(cmd);
	}
	close(pfd[1]);
	dup2(pfd[0], 0);
	close(pfd[0]);
	wait(&status);
	return (0);
}

int	main(int argc, char **argv)
{
	int	infile;
	int	outfile;
	int	i;

	if (argc < 5)
		return (ft_putstr_fd("too few arguments\n", 2), -1);
	infile = get_infile_fd(argc, argv);
	outfile = get_outfile_fd(argc, argv);
	if (infile < 0 || outfile < 0)
		return (close(infile), close(outfile), 0);
	if (!ft_strncmp(argv[1], "here_doc\0", 9))
		i = 3;
	else
		i = 2;
	if (dup2(infile, 0) < 0)
		return (ft_putstr_fd("dup2 failed\n", 2), -1);
	close(infile);
	while (i < argc - 3)
		pipex(argv[i++]);
	if (dup2(outfile, 1) < 0)
		return (ft_putstr_fd("dup2 failed\n", 2), -1);
	execute_command(argv[i]);
	return (close(infile), close(outfile), 0);
}
