/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:59:39 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/24 16:13:21 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_exec_data_fd(t_list *list)
{
	int	i;

	i = -1;
	if (list->exec_data->fd)
	{
		while (++i < (list->size - 1))
		{
			if (list->exec_data->fd[i])
			{
				if (list->exec_data->fd[i][0] != -1)
					close(list->exec_data->fd[i][0]);
				if (list->exec_data->fd[i][1] != -1)
					close(list->exec_data->fd[i][1]);
				free(list->exec_data->fd[i]);
			}
		}
		free(list->exec_data->fd);
	}
	if (list->exec_data->infile != -1)
		close(list->exec_data->infile);
	if (list->exec_data->outfile != -1)
		close(list->exec_data->outfile);
}

void	free_exec_data(t_list *list)
{
	if (!list->exec_data)
		return ;
	if (list->exec_data->processes)
		free(list->exec_data->processes);
	close_exec_data_fd(list);
	if (list->cmd && list->cmd[0].input_redir_type == HEREDOC)
		unlink("here_doc.txt");
	free(list->exec_data);
}

void	free_cmd_argv(t_cmd *cmd)
{
	int	j;

	j = -1;
	if (!cmd)
		return ;
	if (cmd->argv)
	{
		while (cmd->argv[++j])
		{
			if (cmd->argv[j])
				free(cmd->argv[j]);
		}
		free(cmd->argv);
	}
}

void	free_lists(t_list *list)
{
	int	i;

	if (!list)
		return ;
	free_exec_data(list);
	if (!list->cmd)
		return ;
	i = -1;
	while (++i < list->size)
	{
		if (list->cmd[i].argv)
			free_cmd_argv(&list->cmd[i]);
		if (list->cmd[i].delim)
			free(list->cmd[i].delim);
		if (list->cmd[i].infile_name)
			free(list->cmd[i].infile_name);
		if (list->cmd[i].outfile_name)
			free(list->cmd[i].outfile_name);
	}
	free(list->cmd);
}
