/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:41:32 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/25 16:54:21 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wait_for_children(t_appdata *appdata, t_list *list)
{
	int		i;
	pid_t	pid;
	int		status;

	i = 0;
	while (i < list->size)
	{
		pid = waitpid(list->exec_data->processes[i], &status, 0);
		if (pid == -1)
			error_rising(appdata);
		if (WIFEXITED(status))
			list->exec_data->status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			list->exec_data->status = WTERMSIG(status) + SIGNAL_EXIT;
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
		list->exec_data->fd[i][0] = -1;
		list->exec_data->fd[i][1] = -1;
		if (!list->exec_data->fd[i])
			error_rising(appdata);
		if (pipe(list->exec_data->fd[i]) == -1)
			error_rising(appdata);
	}
}

static void	execute_a_list(t_appdata *appdata, t_list *list)
{
	int	i;

	i = -1;
	init_exec_data(list);
	while (++i < list->size)
		list->cmd[i].is_builtin = check_if_builtin(list->cmd[i]);
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

void	start_execution(t_appdata *appdata)
{
	int	i;

	i = 1;
	execute_a_list(appdata, &appdata->lists[0]);
	while (i < appdata->lists_num)
	{
		if (appdata->should_exit == TRUE)
		{
			free_env(appdata->env);
			free_memory(appdata);
			exit(appdata->exit_code);
		}
		if (appdata->lists[i - 1].and_after == TRUE)
		{
			if (appdata->lists[i - 1].exec_data->status == SUCCESS)
				execute_a_list(appdata, &appdata->lists[i]);
		}
		else if (appdata->lists[i - 1].or_after == TRUE)
		{
			if (appdata->lists[i - 1].exec_data->status != SUCCESS)
				execute_a_list(appdata, &appdata->lists[i]);
		}
		i++;
	}
}
