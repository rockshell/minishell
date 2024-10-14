/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:01:32 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/14 16:11:41 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_path(t_cmd *cmd)
{
	int		i;
	char	**paths;
	char	*current_path;
	char	*cmd_with_slash;

	if (cmd->argv[0][0] == '/')
	{
		current_path = ft_strdup(cmd->argv[0]);
		return (current_path);
	}
	i = 0;
	paths = ft_split(getenv("PATH"), ':');
	cmd_with_slash = ft_strjoin("/", cmd->argv[0]);
	while (paths[i])
	{
		current_path = ft_strjoin(paths[i], cmd_with_slash);
		if (access(current_path, F_OK) == 0)
			break ;
		free(current_path);
		i++;
	}
	free(cmd_with_slash);
	return (current_path);
}

void	close_pipes_in_parent(t_list *list)
{
	int	i;

	i = 0;
	while (i < (list->size - 1))
	{
		close(list->exec_data->fd[i][0]);
		close(list->exec_data->fd[i][1]);
		i++;
	}
}

void	redirect_only_child(t_appdata *appdata, t_list *list)
{
	if (list->cmd[0].input_redir_type != 0)
	{
		if (dup2(list->exec_data->infile, 0) == -1)
			error_rising(appdata);
		close(list->exec_data->infile);
	}
	if (list->cmd[0].output_redir_type != 0)
	{
		if (dup2(list->exec_data->outfile, 1) == -1)
			error_rising(appdata);
		close(list->exec_data->outfile);
	}
}

void	execute_a_builtin(t_appdata *appdata, t_cmd *cmd)
{
	if (ft_strncmp(cmd->argv[0], "cd", 2) == 0)
		ft_cd(&cmd, appdata->env);
	else if (ft_strncmp(cmd->argv[0], "echo", 4) == 0)
		ft_echo(&cmd);
	else if (ft_strncmp(cmd->argv[0], "env", 3) == 0)
		ft_env(appdata->env);
	else if (ft_strncmp(cmd->argv[0], "exit", 4) == 0)
		ft_exit(appdata, &cmd);
	else if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		ft_export(&cmd, appdata->env);
	else if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0)
		ft_pwd();
	else if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
		ft_unset(&cmd, appdata->env);
}
