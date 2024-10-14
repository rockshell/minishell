/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:13:45 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/14 14:32:17 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	is_new_line;

	i = 1;
	is_new_line = 1;
	if (ft_strncmp(cmd->argv[i], "-n", ft_strlen(cmd->argv[i])) == 0)
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
	return (0);
}
