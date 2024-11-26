/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:48:47 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/26 16:52:30 by vkinsfat         ###   ########.fr       */
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

int	io_redirection(t_cmd *cmd)
{
	if (cmd->num_of_infiles != 0)
	{
		if (dup2(cmd->infile_fd, 0) == -1)
			return (FAILURE);
	}
	if (cmd->num_of_outfiles != 0)
	{
		if (dup2(cmd->outfile_fd, 1) == -1)
			return (FAILURE);
	}
	return (SUCCESS);
}

//TODO - make it ok!
void	close_fds(t_cmd *cmd, t_exec_data *exec_data, int current_pipe)
{
	int	i;
	int	num_of_cmd;

	i = -1;
	num_of_cmd = *exec_data->num_of_cmd;
	while (++i < num_of_cmd - 1)
	{
		if (i != current_pipe - 1)
			close(exec_data->fd[i][0]);
		if (i != current_pipe)
			close(exec_data->fd[i][1]);
	}
	if (current_pipe != 0 && cmd->infile_fd != -1)
		close(cmd->infile_fd);
	if (current_pipe != (num_of_cmd - 1) && cmd->outfile_fd != -1)
		close(cmd->outfile_fd);
}

void	print_child_error_message(char *cmd_name)
{
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}
