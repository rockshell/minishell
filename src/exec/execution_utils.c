/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:32 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/09/23 16:01:33 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_memory(t_appdata *appdata)
{
	int	i;

	i = -1;
	if (appdata->exec_data->processes)
		free(appdata->exec_data->processes);
	while (++i < appdata->exec_data->pipe_counter)
	{
		close(appdata->exec_data->fd[i][0]);
		close(appdata->exec_data->fd[i][1]);
	}
	if (appdata->exec_data->infile != -1)
		close(appdata->exec_data->infile);
	if (appdata->exec_data->outfile != -1)
		close(appdata->exec_data->outfile);
	free(appdata);
}
	// if (pipex->is_heredoc)
	// 	unlink("here_doc.txt");
	// if (pipex->fd)
	// 	free_fd_array(pipex->fd, pipex->num_of_pipes);
	// if (pipex->paths)
	// 	free_array(pipex->paths);
	//TODO - check for aloccated memory

void	error_rising(t_appdata *appdata)
{
	perror("Error");
	free_memory(appdata);
	exit(1);
}

void	count_service_tokens(t_appdata *appdata)
{
	int			i;
	t_srv_token	*srv_tokens;

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
