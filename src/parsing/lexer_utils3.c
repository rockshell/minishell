/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:53:59 by akulikov          #+#    #+#             */
/*   Updated: 2024/12/02 19:55:44 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_the_list(t_list *list, int start, int end)
{
	list->size = 0;
	list->and_after = 0;
	list->or_after = 0;
	list->end_after = 0;
	if (end - start > 0)
		list->cmd = malloc(sizeof(t_cmd) * (end - start));
	else
		list->cmd = malloc(sizeof(t_cmd) * 1);
	if (!list->cmd)
		return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	list->exec_data = malloc(sizeof(t_exec_data));
	if (!list->exec_data)
		return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	init_exec_data(list);
	return (SUCCESS);
}

int	expand_argument(t_cmd *cmd, t_env *env)
{
	int		i;
	char	*result;
	char	*value;

	i = 0;
	while (i < cmd->argc)
	{
		if (ft_strncmp(cmd->argv[i], "./", 2) == 0)
		{
			value = ft_strdup(cmd->argv[i] + 1);
			if (!value)
				return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
			result = ft_get_env(env, "PWD");
			free(cmd->argv[i]);
			cmd->argv[i] = gnl_strjoin(result, value);
			if (!cmd->argv[i])
				return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

int	expand_exec(t_appdata *appdata, t_env *env)
{
	int	i;
	int	j;

	i = 0;
	while (i < appdata->lists_num)
	{
		j = 0;
		while (j < appdata->lists[i].size)
		{
			if (expand_argument(&appdata->lists[i].cmd[j], env) == FAILURE)
				return (FAILURE);
			j++;
		}
		i++;
	}
	return (SUCCESS);
}