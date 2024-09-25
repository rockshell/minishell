/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:32 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/09/25 16:29:44 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_memory(t_appdata *appdata)
{
	int	i;

	i = -1;
	if (appdata->exec_data->processes)
		free(appdata->exec_data->processes);
	while (++i < appdata->exec_data->pipe_counter)
	{
		close(appdata->exec_data->fd[i][0]);
		close(appdata->exec_data->fd[i][1]);
	}
	if (appdata->exec_data->infile != -1)
		close(appdata->exec_data->infile);
	if (appdata->exec_data->outfile != -1)
		close(appdata->exec_data->outfile);
}
	// if (pipex->is_heredoc)
	// 	unlink("here_doc.txt");
	// if (pipex->fd)
	// 	free_fd_array(pipex->fd, pipex->num_of_pipes);
	// if (pipex->paths)
	// 	free_array(pipex->paths);
	//TODO - check for aloccated memory

void	error_rising(t_appdata *appdata)
{
	perror("Error");
	free_memory(appdata);
	exit(1);
}

void	get_number_of_pipe_and_redirection(t_appdata *appdata)
{
	int			i;
	t_srv_token	*srv_tokens;

	srv_tokens = appdata->srv_tokens;
	i = -1;
	while (++i < appdata->srv_tokens_num)
	{
		if (srv_tokens[i].type == 1)
			appdata->exec_data->pipe_counter++;
		else if (srv_tokens[i].type == 2 || srv_tokens[i].type == 4)
			appdata->exec_data->input_redirection_num++;
		else if (srv_tokens[i].type == 3 || srv_tokens[i].type == 5)
			appdata->exec_data->output_redirection_num++;
	}
}

char	*make_path(t_cmd_token token)
{
	int		i;
	char	**paths;
	char	*current_path;
	char	*cmd_with_slash;
	
	if (token.argv[0][0] == '/')
	{
		current_path = ft_strdup(token.argv[0]);
		return (current_path);
	}
	i = 0;	
	paths = ft_split(getenv("PATH"), ':');
	cmd_with_slash = ft_strjoin("/", token.argv[0]);
	while (paths[i])
	{
		current_path = ft_strjoin(paths[i], cmd_with_slash);
		if (access(current_path, F_OK) == 0)
			break;
		free(current_path);
		i++;
	}
	free(cmd_with_slash);
	return(current_path);
}

void close_pipes_in_parent(t_appdata *appdata)
{
    int i = 0;
    while (i < appdata->exec_data->pipe_counter)
    {
        close(appdata->exec_data->fd[i][0]);  // Close the read end
        close(appdata->exec_data->fd[i][1]);  // Close the write end
        i++;
    }
}