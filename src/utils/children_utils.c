/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:48:47 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/11 17:19:02 by vkinsfat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO - free an array "paths"
char	*make_path(t_cmd *cmd)
{
	int		i;
	char	**paths;
	char	*current_path;
	char	*cmd_with_slash;

	if (cmd->argv[0][0] == '/')
	{
		current_path = ft_strdup(cmd->argv[0]);
		return (current_path);
	}
	i = 0;
	paths = ft_split(getenv("PATH"), ':');
	while (paths[i])
	{
		cmd_with_slash = ft_strjoin("/", cmd->argv[0]);
		current_path = ft_strjoin(paths[i], cmd_with_slash);
		free(cmd_with_slash);
		if (access(current_path, F_OK) == 0)
			return (current_path);
		free(current_path);
		i++;
	}
	return (NULL);
}

void	redirect_only_child(t_appdata *appdata, t_list *list)
{
	if (list->cmd[0].input_redir_type != 0)
	{
		if (dup2(list->exec_data->infile_fd, 0) == -1)
			error_rising(appdata, "dup2");
		close(list->exec_data->infile_fd);
	}
	if (list->cmd[0].output_redir_type != 0)
	{
		if (dup2(list->exec_data->outfile_fd, 1) == -1)
			error_rising(appdata, "dup2");
		close(list->exec_data->outfile_fd);
	}
}

void	io_redirection(t_appdata *appdata, t_list *list, int is_infile)
{
	if (is_infile == 1)
	{
		if (dup2(list->exec_data->infile_fd, 0) == -1)
			error_rising(appdata, "dup2");
	}
	if (is_infile == 0)
	{
		if (dup2(list->exec_data->outfile_fd, 1) == -1)
			error_rising(appdata, "dup2");
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
	if (current_pipe != 0 && list->exec_data->infile_fd != 0)
		close(list->exec_data->infile_fd);
	if (current_pipe != (list->size - 1) && list->exec_data->outfile_fd != 0)
		close(list->exec_data->outfile_fd);
}

void	print_child_error_message(char *cmd_name)
{
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": command not found\n", 2);
	// error_rising(appdata);
	exit(127);
}
