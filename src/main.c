/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:17:26 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/10/10 19:52:40 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO - clear memory
void	save_history(char *cmd)
{
	if (cmd && *cmd)
		add_history(cmd);
}

void print_tokens(t_appdata *appdata)
{
	// int	i;
	t_token *current;

	// i = -1;
	current = appdata->first_token;
	printf("=================\nPrinting tokens one by one\n=================\n");
	while (current)
	{
		printf("Value: %s\n", current->value);
		printf("Type: %i\n", current->type);
		printf("Position: %i\n", current->pos);
		printf("=====================\n");
		current = current->next;
	}
	printf("=====================\nNum of tokens: %i\n", appdata->tokens_num);
	printf("=====================\n");
}

void print_lists(t_appdata *appdata)
{
    int i;
	int j;
	int k;
    t_list *list;
    t_cmd *cmd;

    printf("\n=================\nPrinting lists one by one\n=================\n");

    i = 0;
    while (i < appdata->lists_num)
    {
        list = &appdata->lists[i];
        printf("========== List %d (size: %d) ==========\n", i + 1, list->size);

        j = 0;
        while (j < list->size)
        {
            cmd = &list->cmd[j];
            printf("  Command %d (argc: %d):\n", j + 1, cmd->argc);

            k = 0;
            while (k < cmd->argc)
            {
                printf("    argv[%d]: %s\n", k, cmd->argv[k]);
                k++;
            }

            j++;
        }

        printf("========================================\n");
        i++;
    }
}


int	main(void)
{
	char		*input;
	t_appdata	appdata;
	// t_token		*current;
	// t_list		*list;
	// int		i;
	// int		j;
	
	// i = 0;
	while (1)
	{
		input = readline("minishell: ");
		if (!input)
			break;
		save_history(input);
		run_parsing(input, &appdata);
		print_tokens(&appdata);
		
		free(input);
		run_lexer(&appdata);
		
		// printf("Lists num: %i\n", appdata.lists_num);
		// printf("First list's size: %i\n", appdata.lists[0].size);
		// print_lists(&appdata);
		// start_execution(&appdata);
		// free_memory(&appdata);
		// current = appdata.first_token;
		
		// while (++i < appdata.tokens_num)
		// {
		// 	printf("Value: %s\n", appdata.tokens[i].value);
		// 	printf("Type: %i\n", appdata.tokens[i].type);
		// 	printf("Position: %i\n", appdata.tokens[i].pos);
		// 	printf("=====================\n");
		// }
		
		
	}
	
	return (0);
}