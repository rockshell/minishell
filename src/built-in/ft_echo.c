/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:13:45 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/07 17:00:54 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_cmd_token *token)
{
	int	i;
	int	is_new_line;

	i = 1;
	is_new_line = 1;
	if (ft_strncmp(token->argv[i], "-n", ft_strlen(token->argv[i])) == 0)
	{
		is_new_line = 0;
		i++;
	}
	while (i < token->argc)
	{
		printf("%s", token->argv[i]);
		if (i < token->argc - 1)
			printf(" ");
		i++;
	}
	if (is_new_line == 1)
		printf("\n");
	return (0);
}
