/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:17:26 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/10/08 16:02:02 by akulikov         ###   ########.fr       */
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
	int		i;
	
	i = -1;
	while (1)
	{
		input = readline("minishell: ");
		if (!input)
			break;
		save_history(input);
		run_parsing(input, &appdata);
		// start_execution(&appdata);
		// free_memory(&appdata);
		current = appdata.first_token;
		printf("=================\nPrinting tokens one by one\n=================\n");
		while (current)
		{
			printf("Value: %s\n", current->value);
			printf("Type: %i\n", current->type);
			printf("Position: %i\n", current->pos);
			printf("=====================\n");
			current = current->next;
		}
		printf("=====================\nNum of tokens: %i\n", appdata.tokens_num);
		printf("=====================\n");
		// while (++i < appdata.tokens_num)
		// {
		// 	printf("Value: %s\n", appdata.tokens[i].value);
		// 	printf("Type: %i\n", appdata.tokens[i].type);
		// 	printf("Position: %i\n", appdata.tokens[i].pos);
		// 	printf("=====================\n");
		// }
		
		free(input);
		// run_lexer(&appdata);
	}
	
	return (0);
}