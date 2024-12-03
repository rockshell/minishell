/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:51:15 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/12/03 20:44:22 by vkinsfat         ###   ########.fr       */
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

void interrupt_heredoc_signal(int signum)
{
	g_sig_received = signum;
	ioctl(0, TIOCSTI, "\n");
}

int	rwr_heredoc(t_cmd *cmd, char *delim)
{
	char	*line;
	
	while (1)
	{
		ft_putstr_fd("> ", 1);
		signal(SIGINT, interrupt_heredoc_signal);
		line = get_next_line(0);
		if (g_sig_received)
		{
			free(line);
			close(cmd->infile_fd);
			unlink("here_doc.txt");
			g_sig_received = 0;
			return (FAILURE);
		}
		if (line == NULL)
		{
			ft_putstr_fd(HD_CTRLD, 2);
			ft_putstr_fd(delim, 2);
			ft_putstr_fd("')\n", 2);
			free(line);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
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

void	print_file_error(char *argument)
{
	ft_putstr_fd("minishell: ", 2);
	perror(argument);
}
