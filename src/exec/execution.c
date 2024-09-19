/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 20:07:49 by akulikov          #+#    #+#             */
/*   Updated: 2024/09/19 16:52:50 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	prepare_pipes(t_pipe_data *pipex)
// {
// 	int	i;

// 	i = 0;
// 	pipex->fd = malloc(sizeof(int *) * pipex->num_of_pipes);
// 	if (!pipex->fd)
// 		error_bonus_rising(pipex);
// 	while (i < pipex->num_of_pipes)
// 	{
// 		pipex->fd[i] = malloc(sizeof(int) * 2);
// 		if (!pipex->fd[i])
// 			error_bonus_rising(pipex);
// 		if (pipe(pipex->fd[i]) == -1)
// 			error_bonus_rising(pipex);
// 		i++;
// 	}
// }

// static void	create_processes(t_pipe_data *pipex, int ac, char **av, char **envp)
// {
// 	int	commands;
// 	int	i;

// 	commands = ac - 3 - pipex->is_heredoc;
// 	pipex->processes = malloc(sizeof(pid_t) * commands);
// 	check_access(pipex, ac, av);
// 	call_children(pipex, commands, av, envp);
// 	i = 0;
// 	while (i < pipex->num_of_pipes)
// 	{
// 		close(pipex->fd[i][0]);
// 		close(pipex->fd[i][1]);
// 		i++;
// 	}
// 	i = 0;
// 	while (i < commands)
// 	{
// 		waitpid(pipex->processes[i], &pipex->status, 0);
// 		i++;
// 	}
// }


static void	first_child(t_appdata *appdata)
{
	char	*path;
	char	**cmd;

	if ()
	if (pipex->infile == -1)
		error_bonus_rising(pipex);
	if (dup2(pipex->infile, 0) == -1)
		error_bonus_rising(pipex);
	if (dup2(pipex->fd[0][1], 1) == -1)
		error_bonus_rising(pipex);
	close_fds(pipex, 0);
	cmd = ft_split(command, ' ');
	path = make_command(pipex, cmd);
	if (!path)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		end_bonus_programm(pipex);
		exit(127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		free(path);
		error_bonus_rising(pipex);
	}
}

static void	last_child(t_pipe_data *pipex, char *command, int i, char **envp)
{
	char	*path;
	char	**cmd;

	if (pipex->outfile == -1)
		error_bonus_rising(pipex);
	if (dup2(pipex->fd[i - 1][0], 0) == -1)
		error_bonus_rising(pipex);
	if (dup2(pipex->outfile, 1) == -1)
		error_bonus_rising(pipex);
	close_fds(pipex, pipex->num_of_pipes);
	cmd = ft_split(command, ' ');
	path = make_command(pipex, cmd);
	if (!path)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		end_bonus_programm(pipex);
		exit(127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		free(path);
		error_bonus_rising(pipex);
	}
}

static void	mid_child(t_pipe_data *pipex, char *command, int i, char **envp)
{
	char	*path;
	char	**cmd;

	if (dup2(pipex->fd[i - 1][0], 0) == -1)
		error_bonus_rising(pipex);
	if (dup2(pipex->fd[i][1], 1) == -1)
		error_bonus_rising(pipex);
	close_fds(pipex, i);
	cmd = ft_split(command, ' ');
	path = make_command(pipex, cmd);
	if (!path)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		end_bonus_programm(pipex);
		exit(127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		free(path);
		error_bonus_rising(pipex);
	}
}

void create_processes(t_appdata *appdata)
{
	int	i;

	i = -1;
	appdata->exec_data->processes = malloc(sizeof(pid_t) * appdata->cmd_tokens_num);
	while (++i < appdata->cmd_tokens_num)
	{
		appdata->exec_data->processes[i] = fork();
		if (appdata->exec_data->processes[i] == 0)
		{
			if (i == 0)
				first_child(appdata);
			else if (i == appdata->cmd_tokens_num - 1)
				last_child(appdata, i);
			else
				mid_child(appdata, i);
		}
	}
	i = -1;
	while (++i < appdata->cmd_tokens_num)
		waitpid(appdata->exec_data->processes[i], &appdata->exec_data->status, 0);
}

void	prepare_pipes(t_appdata *appdata)
{
	int	i;

	i = -1;
	appdata->exec_data->fd = malloc(sizeof(int *) * appdata->exec_data->pipe_counter);
	//TODO - error handling
	while (++i < appdata->exec_data->pipe_counter)
	{
		appdata->exec_data->fd[i] = malloc(sizeof(int) * 2);
		pipe(appdata->exec_data->fd[i]);
	}
}


void	parse_service_tokens(t_appdata *appdata)
{
	int	i;
	t_srv_token *srv_tokens;

	srv_tokens = appdata->srv_tokens;
	i = -1;
	while (++i < appdata->srv_tokens_num)
	{
		if (srv_tokens[i].type == 1)
			appdata->exec_data->pipe_counter++;
		else
			appdata->exec_data->redirect_counter++;
	}
	if (appdata->exec_data->pipe_counter > 0)
	{
		prepare_pipes(appdata);
		create_processes(appdata);
	}
	
}