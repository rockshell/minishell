/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:17:26 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/09/24 19:50:18 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	main(int argc, char **argv, char **envp)
int	main(void)
{
	char		*input;
	t_appdata	appdata;

	// (void) argc;
	// (void) argv;
	// appdata.envp = envp;
	while (1)
	{
		input = readline("minishell: ");
		initial_parsing(input, &appdata);
		run_lexer(&appdata);
		start_execution(&appdata);
	}
	return (0);
}