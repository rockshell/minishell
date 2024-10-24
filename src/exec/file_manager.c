/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:07 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/24 15:52:43 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_files(t_list *list, int is_input)
{
	int	res;

	res = 0;
	if (is_input == 1 && list->cmd[0].input_redir_type == STDIN)
		res = open(list->cmd[0].infile_name, O_RDONLY);
	else if (is_input == 1 && list->cmd[0].input_redir_type == HEREDOC)
		res = open("here_doc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (is_input == 0
		&& list->cmd[list->size - 1].output_redir_type == STDOUT)
		res = open(list->cmd[list->size - 1].outfile_name,
				O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (is_input == 0
		&& list->cmd[list->size - 1].output_redir_type == APPEND)
		res = open(list->cmd[list->size - 1].outfile_name,
				O_RDWR | O_CREAT | O_APPEND, 0644);
	return (res);
}

void	rwr_heredoc(t_appdata *appdata, t_list *list, char *delim)
{
	char	*line;

	if (list->exec_data->infile == -1)
		error_rising(appdata);
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line || ft_strncmp(line, delim, ft_strlen(line)) == 0)
			break ;
		write(list->exec_data->infile, line, ft_strlen(line));
		write(list->exec_data->infile, "\n", 1);
		free(line);
	}
	free(line);
	close(list->exec_data->infile);
	list->exec_data->infile = open("here_doc.txt", O_RDONLY, 0664);
	if (!list->exec_data->infile)
	{
		unlink("here_doc.txt");
		error_rising(appdata);
	}
}
