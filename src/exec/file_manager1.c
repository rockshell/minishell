/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:51:46 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/12/02 21:27:46 by vkinsfat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	stdin_processing(t_cmd *cmd, int i)
{
	if (access(cmd->infile_name[i], F_OK) == -1)
		return (print_file_error(cmd->infile_name[i]), FAILURE);
	cmd->infile_fd = open_files(cmd->infile_name[i],
			cmd->input_redir_type[i], 1);
	if (access(cmd->infile_name[i], R_OK) == -1)
		return (print_file_error(cmd->infile_name[i]), FAILURE);
	return (SUCCESS);
}

static int	manage_infiles(t_cmd *cmd)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (++i < cmd->num_of_infiles)
	{
		if (cmd->input_redir_type[i] == STDIN)
		{
			if (stdin_processing(cmd, i) == FAILURE)
				return (FAILURE);
		}
		if (cmd->input_redir_type[i] == HEREDOC)
		{
			cmd->infile_fd = open_files("here_doc.txt", HEREDOC, 1);
			if (rwr_heredoc(cmd, cmd->delim[j]) == FAILURE)
				return (FAILURE);
			j++;
		}
		if (i < cmd->num_of_infiles - 1)
			close(cmd->infile_fd);
	}
	return (SUCCESS);
}

int is_folder_permission_ok(t_cmd *cmd, int i)
{
	char *folder_name;
	struct stat path_stat;

	if (ft_strrchr(cmd->outfile_name[i], '/') == NULL)
	{
		folder_name = ft_strdup(".");
		if (!folder_name)
			return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	}
	else
	{
		folder_name = ft_strdup(cmd->outfile_name[i]);
		if (!folder_name)
			return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
		*ft_strrchr(folder_name, '/') = '\0';
	}
	stat(folder_name, &path_stat);
	if (!S_ISDIR(path_stat.st_mode))
		return (free(folder_name), print_file_error(cmd->outfile_name[i]), 1);
	if (access(folder_name, F_OK | W_OK) == -1)
	{
		print_file_error(cmd->outfile_name[i]);
		return (free(folder_name), FAILURE); 
	}
	free(folder_name);
	return (SUCCESS);
}

//TODO folder permission check
static int	manage_outfiles(t_cmd *cmd)
{
	int	i;

	i = -1;
	while (++i < cmd->num_of_outfiles)
	{
		if (is_folder_permission_ok(cmd, i) == FAILURE)
			return (FAILURE);
		cmd->outfile_fd = open_files(cmd->outfile_name[i],
				cmd->output_redir_type[i], 0);
		if (access(cmd->outfile_name[i], W_OK) == -1)
			return (print_file_error(cmd->outfile_name[i]), FAILURE);
		if (i < cmd->num_of_outfiles - 1)
			close(cmd->outfile_fd);
	}
	return (SUCCESS);
}

static int	run_manage_files(t_cmd *cmd)
{
	if (cmd->num_of_infiles != 0)
	{
		if (manage_infiles(cmd) == FAILURE)
			return (FAILURE);
	}
	if (cmd->num_of_outfiles != 0)
	{
		if (manage_outfiles(cmd) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	file_manager(t_list *list)
{
	int	i;

	i = -1;
	while (++i < list->size)
	{
		if (list->cmd[i].num_of_infiles != 0
			|| list->cmd[i].num_of_outfiles != 0)
		{
			if (run_manage_files(&list->cmd[i]) == FAILURE)
			{
				if (i == (list->size - 1))
				{
					list->exec_data->status = 1;
					return (FAILURE);
				}
			}
		}
	}
	return (SUCCESS);
}
