/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:16 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/14 16:08:56 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_child(t_appdata *appdata, t_list *list)
{
	char	*path;

	if (list->cmd[0].input_redir_type != 0)
		io_redirection(appdata, list, 1);
	if (dup2(list->exec_data->fd[0][1], 1) == -1)
		error_rising(appdata);
	close_fds(list, 0);
	path = make_path(&list->cmd[0]);
	if (!path)
	{
		ft_putstr_fd(list->cmd[0].argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (list->cmd[0].is_builtin == 1)
		execute_a_builtin(appdata, &list->cmd[0]);
	else
	{
		if (execve(path, list->cmd[0].argv, NULL) == -1)
		{
			free(path);
			error_rising(appdata);
		}
	}
}

void	last_child(t_appdata *appdata, t_list *list, int i)
{
	char	*path;

	if (list->cmd[list->size - 1].output_redir_type != 0)
		io_redirection(appdata, list, 0);
	if (dup2(list->exec_data->fd[i - 1][0], 0) == -1)
		error_rising(appdata);
	close_fds(list->exec_data, list->size - 1);
	path = make_path(&list->cmd[i]);
	if (!path)
	{
		ft_putstr_fd(list->cmd[i].argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (list->cmd[i].is_builtin == 1)
		execute_a_builtin(appdata, &list->cmd[i]);
	else
	{
		if (execve(path, list->cmd[i].argv, NULL) == -1)
		{
			free(path);
			error_rising(appdata);
		}
	}
}

void	mid_child(t_appdata *appdata, t_list *list, int i)
{
	char	*path;

	if (dup2(list->exec_data->fd[i - 1][0], 0) == -1)
		error_rising(appdata);
	if (dup2(list->exec_data->fd[i][1], 1) == -1)
		error_rising(appdata);
	close_fds(list->exec_data, i);
	path = make_path(&list->cmd[i]);
	if (!path)
	{
		ft_putstr_fd(list->cmd[i].argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (list->cmd[i].is_builtin == 1)
		execute_a_builtin(appdata, &list->cmd[i]);
	else
	{
		if (execve(path, list->cmd[i].argv, NULL) == -1)
		{
			free(path);
			error_rising(appdata);
		}
	}
}

void	only_child(t_appdata *appdata, t_list *list)
{
	char	*path;

	redirect_only_child(appdata, list);
	path = make_path(&list->cmd[0]);
	if (!path)
	{
		ft_putstr_fd(list->cmd[0].argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (list->cmd[0].is_builtin == 1)
		execute_a_builtin(appdata, &list->cmd[0]);
	else
	{
		if (execve(path, list->cmd[0].argv, NULL) == -1)
		{
			free(path);
			error_rising(appdata);
		}
	}
}
