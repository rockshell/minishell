/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:07 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/09/23 16:15:12 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_files(t_appdata *appdata, int is_input)
{
	int	res;

	res = 0;
	if (is_input == 1 && appdata->srv_tokens[0].type == 2)
		res = open(appdata->cmd_tokens[0].filename, O_RDONLY);
	else if (is_input == 1 && appdata->srv_tokens[0].type == 4)
		res = open("here_doc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (is_input == 0
		&& appdata->srv_tokens[appdata->srv_tokens_num - 1].type == 3)
		res = open(appdata->cmd_tokens[appdata->cmd_tokens_num - 1].filename,
				O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (is_input == 0
		&& appdata->srv_tokens[appdata->srv_tokens_num - 1].type == 5)
		res = open(appdata->cmd_tokens[appdata->cmd_tokens_num - 1].filename,
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
