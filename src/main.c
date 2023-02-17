/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 11:55:15 by avast             #+#    #+#             */
/*   Updated: 2023/02/17 15:50:09 by avast            ###   ########.fr       */
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
		return (NULL);
	i = 0;
	while (i < nb_cmds - 1)
	{
		if (pipe(pipes + (i * 2)) < 0)
			return (error_msg(PIPE), free(pipes), NULL);
		i++;
	}
	return (pipes);
}

int	get_pipex_infos(int argc, char **argv, t_pipex *pipex, t_cmd **list)
{
	pipex->argc = argc;
	pipex->argv = argv;
	pipex->infile = get_infile_fd(argc, argv, list);
	pipex->outfile = get_outfile_fd(argc, argv);
	if (!ft_strncmp(argv[1], "here_doc\0", 9))
	{
		pipex->first_cmd = 3;
		pipex->nb_cmds = argc - 4;
	}
	else
	{
		pipex->first_cmd = 2;
		pipex->nb_cmds = argc - 3;
	}
	pipex->pipes = create_pipes(pipex->nb_cmds);
	if (!pipex->pipes)
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	int		status;
	t_pipex	pipex;
	t_cmd	*list;

	list = NULL;
	if (check_environment() < 0)
		return (-1);
	if (argc < 5)
		return (error_msg(ARG));
	get_pipex_infos(argc, argv, &pipex, &list);
	f_pipex(argc, argv, pipex, &list);
	status = wait_all_pids(&list, pipex.outfile);
	list_free_cmd(&list);
	if (pipex.outfile == PERMISSION_DENIED)
		shell_error_msg(argv[argc - 1], PERMISSION_DENIED);
	if (pipex.outfile == FILE_CREATION)
		shell_error_msg(argv[argc - 1], FILE_CREATION);
	return (status);
}
