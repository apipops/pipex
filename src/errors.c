/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 13:06:31 by avast             #+#    #+#             */
/*   Updated: 2023/01/25 12:35:20 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

void	free_tab(char **tab, int index)
{
	int	i;

	i = 0;
	if (index < 0)
	{
		while (tab[i])
		{
			free(tab[i]);
			i++;
		}
	}
	else
	{
		while (index >= 0)
		{
			free(tab[index]);
			index--;
		}
	}
	free(tab);
}

void	error_msg(char *cmd, int type)
{
	int	i;

	i = 0;
	while (environ[i] && !ft_strnstr(environ[i], "SHELL=/bin/", 11))
		i++;
	ft_putstr_fd(environ[i] + 11, 2);
	if (type == NO_COMMAND)
		ft_putstr_fd(": command not found: ", 2);
	else if (type == NO_FILE)
		ft_putstr_fd(": no such file or directory: ", 2);
	else
		ft_putstr_fd(": permission denied: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("\n", 2);
}
