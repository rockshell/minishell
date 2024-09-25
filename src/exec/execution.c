/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 20:07:49 by akulikov          #+#    #+#             */
/*   Updated: 2024/09/25 16:36:59 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_processes(t_appdata *appdata)
{
	int	i;

	i = -1;
	appdata->exec_data->processes = malloc(sizeof(pid_t) * (appdata->exec_data->pipe_counter + 1));
	if (!appdata->exec_data->processes)
		error_rising(appdata);
	while (++i < (appdata->exec_data->pipe_counter + 1))
	{
		appdata->exec_data->processes[i] = fork();
		if (appdata->exec_data->processes[i] == 0)
		{
			ft_putstr_fd("Child process forked with PID: ", 2);
			ft_putnbr_fd(getpid(), 2);
			ft_putstr_fd("\n", 2);
			if (i == 0)
			{
				first_child(appdata);
				close_pipes_in_parent(appdata);
			}
			else if (i == appdata->exec_data->pipe_counter)
			{
				last_child(appdata, i);
				close_pipes_in_parent(appdata);
			}
			else
			{
				mid_child(appdata, i);
				close_pipes_in_parent(appdata);
			}
		}
		else
		{
			ft_putstr_fd("Parent created child with PID: ", 2);
			ft_putnbr_fd(appdata->exec_data->processes[i], 2);
			ft_putstr_fd("\n", 2);
		}
	}
	ft_putstr_fd("Pipe counter: ", 2);
	ft_putnbr_fd(appdata->exec_data->pipe_counter, 2);
	ft_putstr_fd("\n", 2);
	i = -1;
	i = -1;
	while (++i < (appdata->exec_data->pipe_counter + 1))
	{
		ft_putstr_fd("Waiting for process: ", 2);
		ft_putnbr_fd(appdata->exec_data->processes[i], 2);
		ft_putstr_fd("\n", 2);
		
		// waitpid(appdata->exec_data->processes[i], &appdata->exec_data->status, 0);
		waitpid(-1, NULL, 0);
	}
		//TODO - make a status array and make all po krasote vasche
	// ft_putstr_fd("After waitpid\n", 2);
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
