/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:51:15 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/11/21 15:16:25 by vkinsfat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_files(char *filename, int redir_type, int is_input)
{
	int	res;

	res = 0;
	if (is_input == 1 && redir_type == STDIN)
		res = open(filename, O_RDONLY, 0644);
	else if (is_input == 1 && redir_type == HEREDOC)
		res = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (is_input == 0 && redir_type == STDOUT)
		res = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (is_input == 0 && redir_type == APPEND)
		res = open(filename, O_RDWR | O_CREAT | O_APPEND, 0644);
	return (res);
}

int	rwr_heredoc(t_cmd *cmd, char *delim)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line || ft_strcmp(line, delim) == 0)
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
