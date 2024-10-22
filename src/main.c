/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:17:26 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/10/22 18:01:58 by arch             ###   ########.fr       */
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
	t_token *current;

	current = appdata->first_token;
	printf("=================\nPrinting tokens one by one\n==================\n");
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

t_appdata	init_appdata()
{
	t_appdata	appdata;

	appdata.num_of_input_strings = 0;
	appdata.tokens_num = 0;
	appdata.lists_num = 0;
	appdata.input_strings = NULL;
	appdata.envp = NULL;
	appdata.env = NULL;
	appdata.lists = NULL;
	appdata.tokens = NULL;
	appdata.first_token = NULL;
	appdata.exec_data = NULL;
	return(appdata);
}

int	main(void)
{
	char		*input;
	t_appdata	appdata;

	appdata = init_appdata();
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
		print_lists(&appdata);
	}
	
	return (0);
}