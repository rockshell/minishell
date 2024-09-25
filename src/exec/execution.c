/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 20:07:49 by akulikov          #+#    #+#             */
/*   Updated: 2024/09/25 17:19:26 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_processes(t_appdata *appdata)
{
	int	i;
	int status;
	pid_t pid;

	i = -1;
	appdata->exec_data->processes = malloc(sizeof(pid_t) * (appdata->exec_data->pipe_counter + 1));
	if (!appdata->exec_data->processes)
		error_rising(appdata);
	while (++i < (appdata->exec_data->pipe_counter + 1))
	{
		appdata->exec_data->processes[i] = fork();
		if (appdata->exec_data->processes[i] == 0)
		{
			if (i == 0)
				first_child(appdata);
			else if (i == appdata->exec_data->pipe_counter)
				last_child(appdata, i);
			else
				mid_child(appdata, i);
			exit(0);
		}
	}
	close_pipes_in_parent(appdata);
	i = 0;
	while (i < (appdata->exec_data->pipe_counter + 1))
	{
		status = 0;
		pid = waitpid(appdata->exec_data->processes[i], &status, 0);
		if (pid == -1)
			error_rising(appdata);
		i++;
	}
	//TODO - make a status array and make all po krasote vasche
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
	t_exec_data *exec_data;

	exec_data = appdata->exec_data;
	get_number_of_pipe_and_redirection(appdata);
	if (exec_data->input_redirection_num > 0)
	{
		exec_data->infile = open_files(appdata, 1);
		if (appdata->srv_tokens[0].type == 4)
			rwr_heredoc(appdata, appdata->cmd_tokens[0].delim);
	}
	if (exec_data->output_redirection_num > 0)
		exec_data->outfile = open_files(appdata, 0);
	if (exec_data->pipe_counter > 0)
	{
		prepare_pipes(appdata);
		create_processes(appdata);
	}
	// if (exec_data->pipe_counter == 0 
	// 	&& exec_data->input_redirection_num == 0 
	// 	&& exec_data->output_redirection_num == 0)
	// 	execute_single(appdata);
}
