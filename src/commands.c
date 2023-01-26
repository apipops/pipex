/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 01:56:32 by avast             #+#    #+#             */
/*   Updated: 2023/01/26 18:05:16 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "../libft/libft.h"

char	*ft_strjoin_cmd(char const *s1, char const *s2)
{
	char	*str;
	size_t	size;
	size_t	i;

	if (s1 == 0)
		size = ft_strlen(s2) + 1;
	else if (s2 == 0)
		size = ft_strlen(s1) + 1;
	else
		size = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = (char *)ft_calloc(size + 1, sizeof(char));
	if (!str)
		return (0);
	i = 0;
	while (i < size)
	{
		while (s1 != 0 && *s1 != 0)
			str[i++] = *s1++;
		str[i++] = '/';
		while (s2 != 0 && *s2 != 0)
			str[i++] = *s2++;
	}
	str[i] = '\0';
	return (str);
}

int	check_path(void)
{
	int	nb_va;
	int	path;

	nb_va = 0;
	while (environ[nb_va])
		nb_va++;
	while (environ[path] && !ft_strnstr(environ[path], "PATH=", 5))
		path++;
	if (path == nb_va)
		return (0);
	return (1);

}

char	*get_command_path(char *cmd)
{
	char	**path;
	char	*cmd_path;
	int		i;

	i = 0;
	while (environ[i] && !ft_strnstr(environ[i], "PATH=", 5))
		i++;
	if (!check_path() || !cmd)
		return (NULL);
	path = ft_split((environ[i] + 5), ':');
	i = 0;
	cmd_path = ft_strjoin_cmd(path[i], cmd);
	while (path[i] && access(cmd_path, X_OK) == -1)
	{
		i++;
		free(cmd_path);
		cmd_path = ft_strjoin_cmd(path[i], cmd);
	}
	if (access(cmd_path, X_OK) == 0)
		return (free_tab(path, -1), cmd_path);
	else
		return (free_tab(path, -1), free(cmd_path), NULL);
}

int	execute_command(char *full_cmd)
{
	char	**cmd;
	char	*path;
	t_fork	f;

	cmd = ft_split(full_cmd, 32);
	if (!cmd)
		return (ft_putstr_fd("split failed\n", 2), -1);
	path = get_command_path(cmd[0]);
	if (!path)
		return (error_msg(cmd[0], NO_COMMAND), free_tab(cmd, -1), NO_COMMAND);
	f.pid = fork();
	if (f.pid == -1)
		return (ft_putstr_fd("fork failed\n", 2), -1);
	else if (f.pid == 0)
	{
		if (execve(path, cmd, environ))
			return (ft_putstr_fd("execve failed\n", 2), -1);
	}
	else
		waitpid(f.pid, &f.status, 0);
	return (free(path), free_tab(cmd, -1), 0);
}
