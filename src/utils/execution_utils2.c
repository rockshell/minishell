/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 18:41:25 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/22 21:54:57 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_builtin(t_cmd cmd)
{
	if (ft_strcmp(cmd.argv[0], "cd") == 0)
		return (TRUE);
	else if (ft_strcmp(cmd.argv[0], "echo") == 0)
		return (TRUE);
	else if (ft_strcmp(cmd.argv[0], "env") == 0)
		return (TRUE);
	else if (ft_strcmp(cmd.argv[0], "exit") == 0)
		return (TRUE);
	else if (ft_strcmp(cmd.argv[0], "export") == 0)
		return (TRUE);
	else if (ft_strcmp(cmd.argv[0], "pwd") == 0)
		return (TRUE);
	else if (ft_strcmp(cmd.argv[0], "unset") == 0)
		return (TRUE);
	return (FALSE);
}

void	io_redirection(t_appdata *appdata, t_list *list, int is_infile)
{
	if (is_infile == 1)
	{
		if (list->exec_data->infile == -1)
			error_rising(appdata);
		if (dup2(list->exec_data->infile, 0) == -1)
			error_rising(appdata);
	}
	if (is_infile == 0)
	{
		if (list->exec_data->outfile == -1)
			error_rising(appdata);
		if (dup2(list->exec_data->outfile, 1) == -1)
			error_rising(appdata);
	}
}

void	close_fds(t_list *list, int current_pipe)
{
	int	i;

	i = -1;
	while (++i < list->size - 1)
	{
		if (i != current_pipe - 1)
			close(list->exec_data->fd[i][0]);
		if (i != current_pipe)
			close(list->exec_data->fd[i][1]);
	}
	if (current_pipe != 0 && list->exec_data->infile != 0)
		close(list->exec_data->infile);
	if (current_pipe != (list->size - 1) && list->exec_data->outfile != 0)
		close(list->exec_data->outfile);
}
