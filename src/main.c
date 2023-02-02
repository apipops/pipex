/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 11:55:15 by avast             #+#    #+#             */
/*   Updated: 2023/02/02 10:05:59 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

int	main(int argc, char **argv)
{
	int		files[2];
	int		status;
	t_cmd	*list;

	status = 0;
	if (check_environment() < 0)
		return (-1);
	if (argc < 5)
		return (error_msg(ARG));
	files[INFILE] = get_infile_fd(argc, argv);
	files[OUTFILE] = get_outfile_fd(argc, argv);
	pipex(argc, argv, files, &list);
	wait_all_pids(&list);
	list_free_cmd(&list);
	if (files[OUTFILE] == PERMISSION_DENIED)
		shell_error_msg(argv[argc - 1], PERMISSION_DENIED);
	if (files[OUTFILE] == FILE_CREATION)
		shell_error_msg(argv[argc - 1], FILE_CREATION);
	return (close(files[INFILE]), close(files[INFILE]), status);
}
