/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:17:26 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/10/01 19:48:29 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO - clear memory
void	save_history(char *cmd)
{
	if (cmd && *cmd)
		add_history(cmd);
}

int	main(void)
{
	char		*input;
	t_appdata	appdata;
	t_token		*current;
	
	while (1)
	{
		input = readline("minishell: ");
		if (!input)
			break;
		save_history(input);
		run_parsing(input, &appdata);
		// run_lexer(&appdata);
		// start_execution(&appdata);
		// free_memory(&appdata);
		current = appdata.first_token;
		while (current)
		{
			printf("Value: %s\n", current->value);
			printf("Type: %i\n", current->type);
			printf("Position: %i\n", current->pos);
			printf("=====================\n");
			current = current->next;
		}
		free(input);
	}
	return (0);
}