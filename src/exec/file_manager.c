/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:07 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/11 18:55:14 by vkinsfat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_files(char *filename, int redir_type, int is_input)
{
	int	res;

	res = 0;
	if (is_input == 1 && redir_type == STDIN)
		res = open(filename, O_RDONLY);
	else if (is_input == 1 && redir_type == HEREDOC)
		res = open("here_doc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (is_input == 0 && redir_type == STDOUT)
		res = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (is_input == 0 && redir_type == APPEND)
		res = open(filename, O_RDWR | O_CREAT | O_APPEND, 0644);
	return (res);
}

static int	rwr_heredoc(t_cmd *cmd, char *delim)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line || ft_strncmp(line, delim, ft_strlen(line)) == 0)
			break ;
		write(cmd->infile_fd, line, ft_strlen(line));
		write(cmd->infile_fd, "\n", 1);
		free(line);
	}
	free(line);
	close(cmd->infile_fd);
	cmd->infile_fd = open("here_doc.txt", O_RDONLY, 0664);
	if (cmd->infile_fd == -1)
	{
		unlink("here_doc.txt");
		return (FAILURE);
	}
	return (SUCCESS);
}

void print_file_error(char *argument)
{
	ft_putstr_fd("minishell: ", 2);
	perror(argument);
}

static int manage_infiles(t_cmd *cmd)
{
	int i;
	int j;
	
	i = -1;
	j = 0;
	while (++i < cmd->num_of_infiles)
	{
		if (access(cmd->infile_name[i], F_OK) == -1)
			return (print_file_error(cmd->infile_name[i]), FAILURE);
		cmd->infile_fd = open_files(cmd->infile_name[i], cmd->input_redir_type[i], 1);
		if (access(cmd->infile_name[i], R_OK) == -1)
			return (print_file_error(cmd->infile_name[i]), FAILURE);
		if (cmd->input_redir_type[i] == HEREDOC)
		{
			if (rwr_heredoc(cmd, cmd->delim[j]) == FAILURE)
				return (FAILURE);
			j++;
		}
		if (i < cmd->num_of_infiles - 1)
			close(cmd->infile_fd);
	}
	return (SUCCESS);
}

//TODO folder permission check
static int manage_outfiles(t_cmd *cmd)
{
	int i;
	
	i = -1;
	while (++i < cmd->num_of_outfiles)
	{
		cmd->outfile_fd = open_files(cmd->outfile_name[i], cmd->output_redir_type[i], 0);
		if (access(cmd->outfile_name[i], W_OK) == -1)
			return (print_file_error(cmd->outfile_name[i]), FAILURE);
		if (i < cmd->num_of_outfiles - 1)
			close(cmd->outfile_fd);
	}
	return (SUCCESS);
}

int file_manager(t_cmd *cmd)
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
