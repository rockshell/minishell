/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:17:26 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/09/25 15:57:33 by arch             ###   ########.fr       */
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
		// ft_putstr_fd("We're in main\n", 2);
		free_memory(&appdata);
		free(input);
	}
	return (0);
}