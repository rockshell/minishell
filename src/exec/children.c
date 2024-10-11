/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:16 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/11 14:44:01 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_child(t_appdata *appdata)
{
	char	*path;

	if (appdata->exec_data->input_redirection_num > 0)
		io_redirection(appdata, 1);
	if (dup2(appdata->exec_data->fd[0][1], 1) == -1)
		error_rising(appdata);
	close_fds(appdata->exec_data, 0);
	path = make_path(appdata->cmd_tokens[0]);
	if (!path)
	{
		ft_putstr_fd(appdata->cmd_tokens[0].argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (execve(path, appdata->cmd_tokens[0].argv, NULL) == -1)
	{
		ft_putstr_fd("hey error\n", 2);
		free(path);
		error_rising(appdata);
	}
	exit(0);
}

void	last_child(t_appdata *appdata, int i)
{
	char	*path;

	if (appdata->exec_data->output_redirection_num > 0)
		io_redirection(appdata, 0);
	if (dup2(appdata->exec_data->fd[i - 1][0], 0) == -1)
		error_rising(appdata);
	close_fds(appdata->exec_data, appdata->exec_data->pipe_counter);
	path = make_path(appdata->cmd_tokens[i]);
	if (!path)
	{
		ft_putstr_fd(appdata->cmd_tokens[i].argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (execve(path, appdata->cmd_tokens[i].argv, NULL) == -1)
	{
		ft_putstr_fd("hey error (last)\n", 2);
		free(path);
		error_rising(appdata);
	}
	exit(0);
}

void	mid_child(t_appdata *appdata, int i)
{
	char	*path;

	if (dup2(appdata->exec_data->fd[i - 1][0], 0) == -1)
		error_rising(appdata);
	if (dup2(appdata->exec_data->fd[i][1], 1) == -1)
		error_rising(appdata);
	close_fds(appdata->exec_data, i);
	path = make_path(appdata->cmd_tokens[i]);
	if (!path)
	{
		ft_putstr_fd(appdata->cmd_tokens[i].argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (execve(path, appdata->cmd_tokens[i].argv, NULL) == -1)
	{
		ft_putstr_fd("hey error(mid)\n", 2);
		free(path);
		error_rising(appdata);
	}
	exit(0);
}

void	only_child(t_appdata *appdata)
{
	char	*path;

	if (appdata->exec_data->input_redirection_num > 0)
	{
		dup2(appdata->exec_data->infile, 0);
		close(appdata->exec_data->infile);
	}
	if (appdata->exec_data->output_redirection_num > 0)
	{
		dup2(appdata->exec_data->outfile, 1);
		close(appdata->exec_data->outfile);
	}
	path = make_path(appdata->cmd_tokens[0]);
	if (!path)
	{
		ft_putstr_fd(appdata->cmd_tokens[0].argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (execve(path, appdata->cmd_tokens[0].argv, NULL) == -1)
	{
		ft_putstr_fd("hey error\n", 2);
		free(path);
		error_rising(appdata);
	}
	exit(0);
}
