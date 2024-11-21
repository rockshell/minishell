/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:16 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/21 18:53:21 by vkinsfat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_rising(char *argument)
{
	int			exit_code;
	struct stat	path_stat;

	exit_code = 1;
	stat(argument, &path_stat);
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argument, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit_code = COMMAND_NOT_EXECUTABLE;
	}
	else
	{
		if (errno == EACCES)
			exit_code = COMMAND_NOT_EXECUTABLE;
		else if (errno == ENOENT)
			exit_code = COMMAND_NOT_FOUND;
		else if (errno == ENOMEM)
			exit_code = FAILURE;
		ft_putstr_fd("minishell: ", 2);
		perror(argument);
	}
	exit(exit_code);
}

void	first_child(t_appdata *appdata, t_exec_data *exec_data, t_cmd *cmd)
{
	char	*path;
	int		status;

	status = 0;
	if (io_redirection(cmd) == FAILURE)
		exit (1);
	if (cmd->num_of_outfiles == 0)
	{
		if (dup2(exec_data->fd[0][1], 1) == -1)
			exit(1);
	}
	close_fds(cmd, exec_data, 0);
	if (cmd->is_builtin == TRUE)
	{
		status = execute_a_builtin(appdata, cmd);
		exit(status);
	}
	path = make_path(cmd);
	if (!path)
		print_child_error_message(cmd->argv[0]);
	if (execve(path, cmd->argv, appdata->envp) == -1)
		error_rising(cmd->argv[0]);
}

void	last_child(t_appdata *appdata, t_exec_data *exec_data, t_cmd *cmd, int i)
{
	char	*path;
	int		status;

	status = 0;
	if (io_redirection(cmd) == FAILURE)
		exit (1);
	if (cmd->num_of_infiles == 0)
	{
		if (dup2(exec_data->fd[i - 1][0], 0) == -1)
			exit(1);
	}
	close_fds(cmd, exec_data, i);
	if (cmd->is_builtin == TRUE)
	{
		status = execute_a_builtin(appdata, cmd);
		exit(status);
	}
	path = make_path(cmd);
	if (!path)
		print_child_error_message(cmd->argv[0]);
	if (execve(path, cmd->argv, appdata->envp) == -1)
		error_rising(cmd->argv[0]);
}

void	mid_child(t_appdata *appdata, t_exec_data *exec_data, t_cmd *cmd, int i)
{
	char	*path;
	int		status;

	status = 0;
	if (cmd->num_of_infiles == 0)
	{
		if (dup2(exec_data->fd[i - 1][0], 0) == -1)
			exit(1);
	}
	if (cmd->num_of_outfiles == 0)
	{
		if (dup2(exec_data->fd[i][1], 1) == -1)
			exit(1);
	}
	close_fds(cmd, exec_data, i);
	if (cmd->is_builtin == TRUE)
	{
		status = execute_a_builtin(appdata, cmd);
		exit(status);
	}
	path = make_path(cmd);
	if (!path)
		print_child_error_message(cmd->argv[0]);
	if (execve(path, cmd->argv, appdata->envp) == -1)
		error_rising(cmd->argv[0]);
}

void	only_child(t_appdata *appdata, t_cmd *cmd)
{
	char	*path;

	if (cmd->num_of_infiles != 0)
	{
		if (dup2(cmd->infile_fd, 0) == -1)
			exit(1);
		close(cmd->infile_fd);
	}
	if (cmd->num_of_outfiles != 0)
	{
		if (dup2(cmd->outfile_fd, 1) == -1)
			exit(1);
		close(cmd->outfile_fd);
	}
	path = make_path(cmd);
	if (!path)
		print_child_error_message(cmd->argv[0]);
	if (execve(path, cmd->argv, appdata->envp) == -1)
		error_rising(cmd->argv[0]);
}
