/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 20:07:49 by akulikov          #+#    #+#             */
/*   Updated: 2024/09/23 16:02:06 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_processes(t_appdata *appdata)
{
	int	i;

	i = -1;
	appdata->exec_data->processes = malloc(sizeof(pid_t) * appdata->cmd_tokens_num);
	if (!appdata->exec_data->processes)
		error_rising(appdata);
	while (++i < appdata->cmd_tokens_num)
	{
		appdata->exec_data->processes[i] = fork();
		if (appdata->exec_data->processes[i] == 0)
		{
			if (i == 0)
				first_child(appdata);
			else if (i == appdata->cmd_tokens_num - 1)
				last_child(appdata, i);
			else
				mid_child(appdata, i);
		}
	}
	i = -1;
	while (++i < appdata->cmd_tokens_num)
		waitpid(appdata->exec_data->processes[i], &appdata->exec_data->status, 0);
}

void	prepare_pipes(t_appdata *appdata)
{
	int	i;

	i = -1;
	appdata->exec_data->fd = malloc(sizeof(int *) * appdata->exec_data->pipe_counter);
	if (!appdata->exec_data->fd)
		error_rising(appdata);
	while (++i < appdata->exec_data->pipe_counter)
	{
		appdata->exec_data->fd[i] = malloc(sizeof(int) * 2);
		if (!appdata->exec_data->fd[i])
			error_rising(appdata);
		if (pipe(appdata->exec_data->fd[i]) == -1)
			error_rising(appdata);
	}
}

void	start_execution(t_appdata *appdata)
{
	count_service_tokens(appdata);
	if (appdata->exec_data->redirect_in_counter > 0)
	{
		appdata->exec_data->infile = open_files(appdata, 1);
		if (appdata->srv_tokens[0].type = 4)
			rwr_heredoc(appdata, appdata->cmd_tokens[0].delim);
	}
	if (appdata->exec_data->redirect_out_counter > 0)
		appdata->exec_data->outfile = open_files(appdata, 0);
	if (appdata->exec_data->pipe_counter > 0)
	{
		prepare_pipes(appdata);
		create_processes(appdata);
	}
	if (appdata->exec_data->pipe_counter == 0 
		&& appdata->exec_data->redirect_in_counter == 0 
		&& appdata->exec_data->redirect_out_counter == 0)
		execute_single(appdata);
}
