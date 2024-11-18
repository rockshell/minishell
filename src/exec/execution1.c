/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:41:32 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/18 21:51:52 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_multiple(t_appdata *appdata, t_list *list)
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
	if (file_manager(list) == FAILURE)
		return (FAILURE);
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
