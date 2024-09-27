/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:07 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/09/27 17:04:28 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_files(t_appdata *appdata, int is_input, int i)
{
	int	res;

	res = 0;
	if (is_input == 1
		&& appdata->tokens[i].input_redir_type == IN_REDIR)
		res = open(appdata->tokens[i].infile_name, O_RDONLY);
	else if (is_input == 1
		&& appdata->tokens[i].input_redir_type == HERE_DOC_REDIR)
		res = open("here_doc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (is_input == 0
		&& appdata->tokens[i].output_redir_type == OUT_REDIR)
		res = open(appdata->tokens[i].outfile_name,
				O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (is_input == 0
		&& appdata->tokens[i].output_redir_type == APPEND_REDIR)
		res = open(appdata->tokens[i].outfile_name,
				O_RDWR | O_CREAT | O_APPEND, 0644);
	return (res);
}

void	rwr_heredoc(t_appdata *appdata, char *delim)
{
	char	*line;

	if (appdata->exec_data->infile == -1)
		error_rising(appdata);
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line || ft_strncmp(line, delim, ft_strlen(line)) == 0)
			break ;
		write(appdata->exec_data->infile, line, ft_strlen(line));
		write(appdata->exec_data->infile, "\n", 1);
		free(line);
	}
	free(line);
	close(appdata->exec_data->infile);
	appdata->exec_data->infile = open("here_doc.txt", O_RDONLY, 0664);
	if (!appdata->exec_data->infile)
	{
		unlink("here_doc.txt");
		error_rising(appdata);
	}
}
