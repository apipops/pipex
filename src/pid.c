/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:47:25 by avast             #+#    #+#             */
/*   Updated: 2023/02/17 12:18:39 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

int	list_create_elem(t_cmd *new, char *name, char *path, pid_t pid)
{
	(*new).name = ft_strdup(name);
	(*new).path = ft_strdup(path);
	(*new).pid = pid;
	(*new).next = NULL;
	if (pid == NO_FILE)
		(*new).error = NO_FILE;
	else if (access(path, X_OK) == 0)
		(*new).error = NO_ERROR;
	else if (access(path, F_OK) == 0)
		(*new).error = PERMISSION_DENIED;
	else
		(*new).error = NO_COMMAND;
	return (0);
}

int	list_add_cmd(t_cmd **list, char *name, char *path, pid_t pid)
{
	t_cmd	*new;
	t_cmd	*cur;

	new = malloc(sizeof (t_cmd));
	if (!new)
		return (-1);
	if (list_create_elem(new, name, path, pid) == -1)
		return (-1);
	if (*list == 0)
	{
		*list = new;
		return (0);
	}
	cur = *list;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
	return (0);
}

int	wait_all_pids(t_cmd **list, int outfile)
{
	t_cmd	*cur;
	int		status;

	status = 0;
	cur = *list;
	while (cur->next)
	{
		if (cur->pid > 0)
			waitpid(cur->pid, NULL, 0);
		else
			unlink(cur->name);
		shell_error_msg(cur->name, cur->error);
		cur = cur->next;
	}
	waitpid(cur->pid, &status, 0);
	shell_error_msg(cur->name, cur->error);
	return (get_return_value(&status, outfile));
}

void	list_free_cmd(t_cmd **list)
{
	t_cmd	*tmp;

	if (!list || !*list)
		return ;
	while (*list)
	{
		tmp = *list;
		*list = (*list)->next;
		if (tmp->name)
			free(tmp->name);
		if (tmp->path)
			free(tmp->path);
		free(tmp);
	}
}

int	show_pids(t_cmd	**list)
{
	t_cmd	*cur;
	int		i;

	i = 1;
	cur = *list;
	while (cur)
	{
		ft_printf("Cmd #%d : %s (pid = %d, error = %d)\n", i, cur->name, cur->pid, cur->error);
		i++;
		cur = cur->next;
	}
	return (0);
}

int	get_return_value(int *status, int outfile)
{
	if (outfile < 0)
		return (1);
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	else
		return (0);
}
