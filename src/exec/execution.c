/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 20:07:49 by akulikov          #+#    #+#             */
/*   Updated: 2024/10/15 15:25:34 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO - make a status array and make all po krasote vasche
static void	create_processes(t_appdata *appdata, t_list *list)
{
	int		i;
	int		status;
	pid_t	pid;

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
	i = 0;
	while (i < list->size)
	{
		status = 0;
		pid = waitpid(list->exec_data->processes[i], &status, 0);
		if (pid == -1)
			error_rising(appdata);
		i++;
	}
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

	status = 0;
	pid = fork();
	if (pid == -1)
		error_rising(appdata);
	if (pid == 0)
		only_child(appdata, list);
	waitpid(pid, &status, 0);
}

void	start_execution(t_appdata *appdata, t_list *list)
{
	t_exec_data	*exec_data;
	int			i;

	i = -1;
	exec_data = list->exec_data;
	while (++i < list->size)
		list->cmd[i].is_builtin = check_if_builtin(&list->cmd[i]);
	if (list->cmd[0].input_redir_type != 0)
	{
		if (list->cmd[0].input_redir_type == STDIN)
			exec_data->infile = open_files(list, 1);
		else if (list->cmd[0].input_redir_type == HEREDOC)
			rwr_heredoc(appdata, list, list->cmd[0].delim);
	}
	if (list->cmd[list->size -1].output_redir_type != 0)
		exec_data->outfile = open_files(list, 0);
	if (list->size > 1)
	{
		prepare_pipes(appdata, list);
		create_processes(appdata, list);
	}
	else
		execute_single(appdata, list);
}
