/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:41:10 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/04 23:16:55 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_a_builtin(t_appdata *appdata, t_cmd *cmd)
{
	int	exit_status;

	exit_status = 0;
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		exit_status = ft_cd(cmd, appdata->env);
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		exit_status = ft_echo(cmd);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		exit_status = ft_env(appdata->env);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		exit_status = ft_exit(appdata, cmd);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
	{
		exit_status = ft_export(cmd, appdata->env);
		update_envp_array(appdata, appdata->env);
	}
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		exit_status = ft_pwd();
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
	{
		exit_status = ft_unset(cmd, appdata->env);
		update_envp_array(appdata, appdata->env);
	}
	return (exit_status);
}

void	execute_single(t_appdata *appdata, t_list *list)
{
	pid_t	pid;
	int		status;

	if (list->cmd[0].is_builtin == FALSE)
	{
		pid = fork();
		if (pid == -1)
			error_rising(appdata, "fork");
		if (pid == 0)
			only_child(appdata, list);
		if (waitpid(pid, &status, 0) == -1)
			error_rising(appdata, "waitpid");
		if (WIFEXITED(status))
			list->exec_data->status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			list->exec_data->status = WTERMSIG(status) + SIGNAL_EXIT;
	}
	else
	{
		list->exec_data->status = execute_a_builtin(appdata, &list->cmd[0]);
		return ;
	}
}
