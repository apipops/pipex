/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:47:25 by avast             #+#    #+#             */
/*   Updated: 2023/02/16 18:33:06 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

void	list_add_cmd(t_cmd **list, char *name, char *path, pid_t pid)
{
	t_cmd	*new;
	t_cmd	*cur;

	new = malloc(sizeof (t_cmd));
	if (!new)
		return ;
	(*new).name = ft_strdup(name);
	(*new).path = ft_strdup(path);
	(*new).pid = pid;
	(*new).next = NULL;
	if (access(path, X_OK) == 0)
		(*new).error = NO_ERROR;
	else if (access(path, F_OK) == 0)
		(*new).error = PERMISSION_DENIED;
	else
		(*new).error = NO_COMMAND;
	if (*list == 0)
	{
		*list = new;
		return ;
	}
	cur = *list;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

int	wait_all_pids(t_cmd **list, int outfile)
{
	t_cmd	*cur;
	int		status;

	status = 0;
	cur = *list;
	while (cur->next)
	{
		waitpid(cur->pid, NULL, WUNTRACED);
		//printf("%s : error = %d\n", cur->name, cur->error);
		shell_error_msg(cur->name, cur->error);
		cur = cur->next;
	}
	waitpid(cur->pid, &status, 0);
	shell_error_msg(cur->name, cur->error);
	return (get_return_value(&status, outfile));
}

int	get_pid_list_size(t_cmd **list)
{
	t_cmd	*cur;
	int		size;

	if (!list)
		return (0);
	cur = *list;
	size = 0;
	while (cur)
	{
		size++;
		cur = cur->next;
	}
	return (size);
}

int	show_pids(t_cmd **list)
{
	t_cmd	*cur;
	int		i;

	if (list == NULL)
		ft_printf("PID list nulle\n");
	cur = *list;
	i = 0;
	while (cur)
	{
		ft_printf("pid %d (id : %d) = commande %s\n", i, (int)cur->pid, cur->name);
		cur = cur->next;
		i++;
	}
	return (0);
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
