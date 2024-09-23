/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:16 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/09/23 16:38:54 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_fds(t_exec_data *exec_data, int current_pipe)
{
	int	i;

	i = -1;
	while (++i < exec_data->pipe_counter)
	{
		if (i != current_pipe - 1)
			close(exec_data->fd[i][0]);
		if (i != current_pipe)
			close(exec_data->fd[i][1]);
	}
	if (current_pipe != 0 && exec_data->input_redirection_num)
		close(exec_data->infile);
	if (current_pipe != exec_data->pipe_counter && exec_data->output_redirection_num)
		close(exec_data->outfile);
}

void io_redirection(t_appdata *appdata, int is_infile)
{
	if (is_infile == 1)
	{
		if (appdata->exec_data->infile == -1)
			error_rising(appdata);
		if (dup2(appdata->exec_data->infile, 0) == -1)
			error_rising(appdata);
	}
	if (is_infile == 0)
	{
		if (appdata->exec_data->outfile == -1)
			error_rising(appdata);
		if (dup2(appdata->exec_data->outfile, 1) == -1)
			error_rising(appdata);
	}
}

void	first_child(t_appdata *appdata)
{
	char	*path;
	// char 	**env;

	// env = getenv(env);
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
	if (execve(path, appdata->cmd_tokens[0].argv, appdata->envp) == -1)
	{
		free(path);
		// free(env);
		error_rising(appdata);
	}
}

void	last_child(t_appdata *appdata, int i)
{
	char	*path;
	// char	*env;

	// env = getenv(env);
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
	if (execve(path, appdata->cmd_tokens[i].argv, appdata->envp) == -1)
	{
		free(path);
		// free(env);
		error_rising(appdata);
	}
}

void	mid_child(t_appdata *appdata, int i)
{
	char	*path;
	// char 	**env;
	
	if (dup2(appdata->exec_data->fd[i - 1][0], 0) == -1)
		error_rising(appdata);
	if (dup2(appdata->exec_data->fd[i][1], 1) == -1)
		error_rising(appdata);
	// env = getenv(env);
	close_fds(appdata->exec_data, i);
	path = make_path(appdata->cmd_tokens[i]);
	if (!path)
	{
		ft_putstr_fd(appdata->cmd_tokens[i].argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (execve(path, appdata->cmd_tokens[i].argv, appdata->envp) == -1)
	{
		free(path);
		// free(env);
		error_rising(appdata);
	}
}
