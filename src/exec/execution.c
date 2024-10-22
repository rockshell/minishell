/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 20:07:49 by akulikov          #+#    #+#             */
/*   Updated: 2024/10/22 21:37:12 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO - make a status array and make all po krasote vasche
static void wait_for_children(t_appdata *appdata, t_list *list)
{
	int i;
	pid_t	pid;
	
	i = 0;
	while (i < list->size)
	{
		pid = waitpid(list->exec_data->processes[i], &list->exec_data->status, 0);
		if (pid == -1)
			error_rising(appdata);
		i++;
	}
}

static void	create_processes(t_appdata *appdata, t_list *list)
{
	int	i;

	i = -1;
	list->exec_data->processes = malloc(sizeof(pid_t) * list->size);
	if (!list->exec_data->processes)
		error_rising(appdata);
	while (++i < list->size)
	{
		list->exec_data->processes[i] = fork();
		if (list->exec_data->processes[i] == 0)
		{
			if (i == 0)
				first_child(appdata, list);
			else if (i == (list->size - 1))
				last_child(appdata, list, i);
			else
				mid_child(appdata, list, i);
		}
	}
	close_pipes_in_parent(list);
}

static void	prepare_pipes(t_appdata *appdata, t_list *list)
{
	int	i;

	i = -1;
	list->exec_data->fd = malloc(sizeof(int *) * (list->size - 1));
	if (!list->exec_data->fd)
		error_rising(appdata);
	while (++i < (list->size - 1))
	{
		list->exec_data->fd[i] = malloc(sizeof(int) * 2);
		if (!list->exec_data->fd[i])
			error_rising(appdata);
		if (pipe(list->exec_data->fd[i]) == -1)
			error_rising(appdata);
	}
}

static void	execute_single(t_appdata *appdata, t_list *list)
{
	pid_t	pid;
	int		status;

	if (list->cmd[0].is_builtin == FALSE)
	{
		pid = fork();
		if (pid == -1)
			error_rising(appdata);
		if (pid == 0)
			only_child(appdata, list);
		if (waitpid(pid, &list->exec_data->status, 0) == -1)
			error_rising(appdata);
	}
	else
	{
		status = execute_a_builtin(appdata, &list->cmd[0]);
		list->exec_data->status = status;
	}
}

void init_exec_data(t_list *list)
{
	list->exec_data->infile = -1;
	list->exec_data->outfile = -1;
	list->exec_data->status = 0;
	list->exec_data->fd = NULL;
	list->exec_data->processes = NULL;
}

void	start_execution(t_appdata *appdata, t_list *list)
{
	int	i;

	i = -1;
	init_exec_data(list);
	while (++i < list->size)
		list->cmd[i].is_builtin = check_if_builtin(&list->cmd[i]);
	if (list->cmd[0].input_redir_type != 0)
	{
		if (list->cmd[0].input_redir_type == STDIN)
			list->exec_data->infile = open_files(list, 1);
		else if (list->cmd[0].input_redir_type == HEREDOC)
			rwr_heredoc(appdata, list, list->cmd[0].delim);
	}
	if (list->cmd[list->size -1].output_redir_type != 0)
		list->exec_data->outfile = open_files(list, 0);
	if (list->size > 1)
	{
		prepare_pipes(appdata, list);
		create_processes(appdata, list);
		wait_for_children(appdata, list);
	}
	else
		execute_single(appdata, list);
}
