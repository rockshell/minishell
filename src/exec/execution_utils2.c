/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 18:41:25 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/11 14:48:59 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_builtin(t_cmd_token *token)
{
	size_t	len;

	len = ft_strlen(token->argv[0]);
	if (ft_strncmp(token->argv[0], "cd", len) == 0)
		return (1);
	else if (ft_strncmp(token->argv[0], "echo", len) == 0)
		return (1);
	else if (ft_strncmp(token->argv[0], "env", len) == 0)
		return (1);
	else if (ft_strncmp(token->argv[0], "exit", len) == 0)
		return (1);
	else if (ft_strncmp(token->argv[0], "export", len) == 0)
		return (1);
	else if (ft_strncmp(token->argv[0], "pwd", len) == 0)
		return (1);
	else if (ft_strncmp(token->argv[0], "unset", len) == 0)
		return (1);
	return (0);
}

void	io_redirection(t_appdata *appdata, int is_infile)
{
	if (is_infile == 1)
	{
		if (appdata->exec_data->infile == -1)
			error_rising(appdata);
		if (dup2(appdata->exec_data->infile, 0) == -1)
			error_rising(appdata);
	}
	if (is_infile == 0)
	{
		if (appdata->exec_data->outfile == -1)
			error_rising(appdata);
		if (dup2(appdata->exec_data->outfile, 1) == -1)
			error_rising(appdata);
	}
}

void	close_fds(t_exec_data *exec_data, int current_pipe)
{
	int	i;

	i = -1;
	while (++i < exec_data->pipe_counter)
	{
		if (i != current_pipe - 1)
			close(exec_data->fd[i][0]);
		if (i != current_pipe)
			close(exec_data->fd[i][1]);
	}
	if (current_pipe != 0 && exec_data->input_redirection_num)
		close(exec_data->infile);
	if (current_pipe != exec_data->pipe_counter
		&& exec_data->output_redirection_num)
		close(exec_data->outfile);
}
