#include "minishell.h"

static void	close_fds(t_exec_data *exec_data, int current_pipe)
{
	int	i;

	i = -1;
	while (++i < exec_data->pipe_counter)
	{
		if (i != current_pipe - 1)
			close(exec_data->fd[i][0]);
		if (i != current_pipe)
			close(exec_data->fd[i][1]);
	}
	if (current_pipe != 0 && exec_data->redirect_in_counter)
		close(exec_data->infile);
	if (current_pipe != exec_data->pipe_counter && exec_data->redirect_out_counter)
		close(exec_data->outfile);
}

void	first_child(t_appdata *appdata)
{
	char	*path;
	char 	**env;

	if (appdata->exec_data->redirect_in_counter > 0)
	{
		if (appdata->exec_data->infile == -1)
			error_rising(appdata);
		if (dup2(appdata->exec_data->infile, 0) == -1)
			error_rising(appdata);
	}
	env = getenv(env);
	if (dup2(appdata->exec_data->fd[0][1], 1) == -1)
		error_rising(appdata);
	close_fds(appdata->exec_data, 0);
	path = make_command(appdata->cmd_tokens[0].cmd);
	if (!path)
	{
		ft_putstr_fd(appdata->cmd_tokens[0].cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (execve(path, appdata->cmd_tokens[0].cmd, env) == -1)
	{
		free(path);
		free(env);
		error_rising(appdata);
	}
}

void	last_child(t_appdata *appdata, int i)
{
	char	*path;
	char	*env;

	if (appdata->exec_data->redirect_out_counter > 0)
	{
		if (appdata->exec_data->outfile == -1)
			error_rising(appdata);
		if (dup2(appdata->exec_data->outfile, 1) == -1)
			error_rising(appdata);
	}
	env = getenv(env);
	if (dup2(appdata->exec_data->fd[i - 1][0], 0) == -1)
		error_rising(appdata);
	close_fds(appdata->exec_data, appdata->exec_data->pipe_counter);
	path = make_command(appdata->cmd_tokens[i].cmd);
	if (!path)
	{
		ft_putstr_fd(appdata->cmd_tokens[i].cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (execve(path, appdata->cmd_tokens[i].cmd, env) == -1)
	{
		free(path);
		free(env);
		error_rising(appdata);
	}
}

void	mid_child(t_appdata *appdata, int i)
{
	char	*path;
	char 	**env;
	
	if (dup2(appdata->exec_data->fd[i - 1][0], 0) == -1)
		error_rising(appdata);
	if (dup2(appdata->exec_data->fd[i][1], 1) == -1)
		error_rising(appdata);
	env = getenv(env);
	close_fds(appdata->exec_data, i);
	path = make_command(appdata->cmd_tokens[i].cmd);
	if (!path)
	{
		ft_putstr_fd(appdata->cmd_tokens[i].cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		error_rising(appdata);
		exit(127);
	}
	if (execve(path, appdata->cmd_tokens[i].cmd, env) == -1)
	{
		free(path);
		free(env);
		error_rising(appdata);
	}
}
