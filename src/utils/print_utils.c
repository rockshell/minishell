/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 22:04:14 by arch              #+#    #+#             */
/*   Updated: 2024/10/29 22:08:15 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_appdata *appdata)
{
	t_token	*current;

	current = appdata->first_token;
	printf("=================\nPrinting tokens one by one\n==================\n");
	while (current)
	{
		printf("Value: %s\n", current->value);
		printf("Type: %i\n", current->type);
		printf("Position: %i\n", current->pos);
		printf("Is expandable: ");
		if (current->needs_expanding == 1)
			printf("YES\n");
		else
			printf("NO\n");
		
		printf("=====================\n");
		current = current->next;
	}
	printf("=====================\nNum of tokens: %i\n", appdata->tokens_num);
	printf("=====================\n");
}

void	print_lists(t_appdata *appdata)
{
	int		i;
	int		j;
	int		k;
	t_list	*list;
	t_cmd	*cmd;

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