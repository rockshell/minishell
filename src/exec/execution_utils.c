/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:32 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/09/30 15:01:16 by akulikov         ###   ########.fr       */
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

void	get_number_of_pipes(t_appdata *appdata)
{
	int	i;

	i = -1;
	while (++i < appdata->tokens_num)
	{
		if (appdata->tokens[i].is_pipe_after == TRUE)
			appdata->exec_data->pipe_counter++;
		// else if (appdata->tokens[i].input_redir_type == IN_REDIR 
		// 	|| appdata->tokens[i].input_redir_type == HERE_DOC_REDIR)
		// 	appdata->exec_data->input_redirection_num++;
		// else if (appdata->tokens[i].input_redir_type  == OUT_REDIR
		// 	|| appdata->tokens[i].input_redir_type  == APPEND_REDIR)
		// 	appdata->exec_data->output_redirection_num++;
	}
}

char	*make_path(t_cmd cmd)
{
	int		i;
	char	**paths;
	char	*current_path;
	char	*cmd_with_slash;
	
	if (cmd.argv[0][0] == '/')
	{
		current_path = ft_strdup(cmd.argv[0]);
		return (current_path);
	}
	i = 0;	
	paths = ft_split(getenv("PATH"), ':');
	cmd_with_slash = ft_strjoin("/", cmd.argv[0]);
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