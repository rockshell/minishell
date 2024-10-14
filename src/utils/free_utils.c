/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:29:24 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/14 16:49:38 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_exec_data(t_list *list)
{
	int	i;

	i = -1;
	if (list->exec_data->processes)
		free(list->exec_data->processes);
	while (++i < (list->size - 1))
	{
		close(list->exec_data->fd[i][0]);
		close(list->exec_data->fd[i][1]);
		free(list->exec_data->fd[i]);
	}
	free(list->exec_data->fd);
	if (list->exec_data->infile != -1)
		close(list->exec_data->infile);
	if (list->exec_data->outfile != -1)
		close(list->exec_data->outfile);
	if (list->cmd[0].input_redir_type == 4)
		unlink("here_doc.txt");
}

void	free_lists(t_list *list)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	free_exec_data(list);
	while (++i < list->size)
	{
		if (list->cmd[i].argv)
		{
			while (list->cmd[i].argv[++j])
				free(list->cmd[i].argv[j]);
			free(list->cmd[i].argv);
		}
		if (list->cmd[i].delim)
			free(list->cmd[i].delim);
		if (list->cmd[i].infile_name)
			free(list->cmd[i].infile_name);
		if (list->cmd[i].outfile_name)
			free(list->cmd[i].outfile_name);
		//free(list->cmd[i]) if it was allocated
	}
	//free(list) if it was allocated
}

void	free_env(t_env *env)
{
	t_env	*temp;

	if (!env)
		return ;
	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
	env = NULL;
}

void	free_memory(t_appdata *appdata)
{
	int	i;

	i = -1;
	while (i < appdata->lists_num)
		free_lists(&appdata->lists[i]);
	// free_tokens(appdata->tokens);
	free_env(appdata->env);
}

void	error_rising(t_appdata *appdata)
{
	perror("Error");
	free_memory(appdata);
	exit(1);
}
