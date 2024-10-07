/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:17:26 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/10/03 14:57:51 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	main(void)
int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_appdata	appdata;

	(void) argc;
	(void) argv;
	initialize_env_var(&appdata, envp);
	while (1)
	{
		input = readline("minishell: ");
		initial_parsing(input, &appdata);
		run_lexer(&appdata);
		start_execution(&appdata);
		// ft_putstr_fd("We're in main\n", 2);
		// free_memory(&appdata);
		free(input);
	}
	return (0);
}