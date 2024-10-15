/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:13:45 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/15 17:12:09 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	is_new_line;

	i = 1;
	is_new_line = 1;
	if (cmd->argc == 1)
	{
		printf("\n");
		return (SUCCESS);
	}
	while (ft_strcmp(cmd->argv[i], "-n") == 0)
	{
		is_new_line = 0;
		i++;
	}
	while (i < cmd->argc)
	{
		printf("%s", cmd->argv[i]);
		if (i < cmd->argc - 1)
			printf(" ");
		i++;
	}
	if (is_new_line == 1)
		printf("\n");
	return (SUCCESS);
}
