#include "minishell.h"

void free_memory(t_appdata *appdata)
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
	// if (pipex->fd)
	// 	free_fd_array(pipex->fd, pipex->num_of_pipes);
	// if (pipex->paths)
	// 	free_array(pipex->paths);
	if (appdata->exec_data->infile != -1)
		close(appdata->exec_data->infile);
	if (appdata->exec_data->outfile != -1)
		close(appdata->exec_data->outfile);
	// if (pipex->is_heredoc)
	// 	unlink("here_doc.txt");
	//TODO - check for aloccated memory
	free(appdata);
}

void error_rising(t_appdata *appdata)
{
	perror("Error");
	free_memory(appdata);
	exit(1);
}