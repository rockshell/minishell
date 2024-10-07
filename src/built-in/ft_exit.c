/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:15:51 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/07 15:52:01 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void run_exit(t_appdata *appdata, t_cmd_token *token)
{
	long long exit;

	exit = 0;
	if (is_valid_digit(token->argv[1]) == 0)
	{
		appdata->exit_code = 2;
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(token->argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
	}
	else
	{
		exit = ft_atoll(token->argv[1]);
		if (exit < 0 || exit > 255)
			appdata->exit_code = (int)(exit % 256);
		else 
			appdata->exit_code = exit;
	}
}

int ft_exit(t_appdata *appdata, t_cmd_token *token)
{
	ft_putstr_fd("exit\n", 1);
	if (token->argc == 1)
		appdata->exit_code = 0;
	else if (token->argc > 2)
	{
		appdata->exit_code = 1;
		return (ft_putstr_fd(EXIT_TOO_MANY_ARG_ERROR, 2), 1);
	}
	else if (token->argc == 2)
		run_exit(appdata, token);
	return (0);
}
