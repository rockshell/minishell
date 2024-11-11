/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:41:32 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/11 18:54:46 by vkinsfat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO waitpid error handling
static int	wait_for_children(t_list *list)
{
	int		i;
	// pid_t	pid;
	int		status;

	i = 0;
	while (i < list->size)
	{
		// pid = waitpid(list->exec_data->processes[i], &status, 0);
		waitpid(list->exec_data->processes[i], &status, 0);
		// if (pid == -1)
		// error_rising(appdata, "waitpid");
		if (i == list->size - 1)
		{
			if (WIFEXITED(status))
				list->exec_data->status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				list->exec_data->status = WTERMSIG(status) + SIGNAL_EXIT;
		}
		i++;
	}
	return (SUCCESS);
}

static int	create_processes(t_appdata *appdata, t_list *list)
{
	int	i;

	i = -1;
	list->exec_data->processes = malloc(sizeof(pid_t) * list->size);
	if (!list->exec_data->processes)
		return(ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	while (++i < list->size)
	{
		list->exec_data->processes[i] = fork();
		if (list->exec_data->processes[i] == 0)
		{
			if (i == 0)
				first_child(appdata, list->exec_data, &list->cmd[0]);
			else if (i == (list->size - 1))
				last_child(appdata, list->exec_data, &list->cmd[i], i);
			else
				mid_child(appdata, list->exec_data, &list->cmd[i], i);
		}
	}
	close_pipes_in_parent(list);
	return (SUCCESS);
}

//TODO pipe error handling
static int	prepare_pipes(t_list *list)
{
	int	i;

	i = -1;
	list->exec_data->fd = malloc(sizeof(int *) * (list->size - 1));
	if (!list->exec_data->fd)
		return(ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	while (++i < (list->size - 1))
	{
		list->exec_data->fd[i] = malloc(sizeof(int) * 2);
		if (!list->exec_data->fd[i])
			return(ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
		list->exec_data->fd[i][0] = -1;
		list->exec_data->fd[i][1] = -1;
		pipe(list->exec_data->fd[i]);
			// error_rising(appdata, "pipe");
	}
	return (SUCCESS);
}

int execute_multiple(t_appdata *appdata, t_list *list)
{
	if (prepare_pipes(list) == FAILURE)
	{
		list->exec_data->status = 1;
		return (FAILURE);
	}
	if (create_processes(appdata, list) == FAILURE)
	{
		list->exec_data->status = 1;
		return (FAILURE);
	}
	if (wait_for_children(list) == FAILURE)
	{
		list->exec_data->status = 1;
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	execute_a_list(t_appdata *appdata, t_list *list)
{
	int	i;

	i = -1;
	while (++i < list->size)
		list->cmd[i].is_builtin = check_if_builtin(list->cmd[i]);
	i = -1;
	while (++i < list->size)
	{
		if (list->cmd[i].num_of_infiles != 0 || list->cmd[i].num_of_outfiles != 0)
		{
			if (file_manager(&list->cmd[i]) == FAILURE)
			{
				list->exec_data->status = 1;
				return (FAILURE);
			}
		}
	}
	if (list->size > 1)
	{
		if (execute_multiple(appdata, list) == FAILURE)
			return (FAILURE);
	}
	else if (list->size == 1)
		execute_single(appdata, list);
	return (SUCCESS);
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
			free_envp_array(appdata->envp);
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
