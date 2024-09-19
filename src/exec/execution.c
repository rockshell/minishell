/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 20:07:49 by akulikov          #+#    #+#             */
/*   Updated: 2024/09/19 17:40:33 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void create_processes(t_appdata *appdata)
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

void	parse_service_tokens(t_appdata *appdata)
{
	int	i;
	t_srv_token *srv_tokens;

	srv_tokens = appdata->srv_tokens;
	i = -1;
	while (++i < appdata->srv_tokens_num)
	{
		if (srv_tokens[i].type == 1)
			appdata->exec_data->pipe_counter++;
		else if (srv_tokens[i].type == 2 || srv_tokens[i].type == 4)
			appdata->exec_data->redirect_in_counter++;
		else if (srv_tokens[i].type == 3 || srv_tokens[i].type == 5)
			appdata->exec_data->redirect_out_counter++;
	}
}

void start_execution(t_appdata *appdata)
{
	parse_service_tokens(appdata);
	if (appdata->exec_data->pipe_counter > 0)
	{
		prepare_pipes(appdata);
		create_processes(appdata);
	}
	// if (appdata->exec_data->redirect_in_counter > 0)
	// {
		
	// }
}