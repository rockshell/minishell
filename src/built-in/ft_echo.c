/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:13:45 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/08/30 15:07:27 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(char **arguments)
{
	int	argc;

	argc = 0;
	while (arguments[argc])
		argc++;
	if (argc == 2)
		printf("%s\n", arguments[1]);
	else if (argc == 3 && ft_strncmp(arguments[1], "-n", 2) == 0)
		printf("%s", arguments[2]);
}
