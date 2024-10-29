/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:17:26 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/10/29 23:16:01 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO - clear memory
void	save_history(char *cmd)
{
	if (cmd && *cmd)
		add_history(cmd);
}

void update_exit_code(t_appdata *appdata)
{
	int last_index;
	int exit_code;
	
	last_index = appdata->lists_num - 1;
	if (appdata->lists)
	{
		exit_code = appdata->lists[last_index].exec_data->status;
		free(appdata->exit_status->value);
		appdata->exit_status->value = ft_itoa(exit_code);
	}
	else
	{
		exit_code = appdata->exit_code;
		free(appdata->exit_status->value);
		appdata->exit_status->value = ft_itoa(exit_code);
		appdata->exit_code = 0;
	}
}

void	new_cycle_preparation(t_appdata *appdata)
{
	int	exit_code;

	exit_code = 0;
	if (appdata->should_exit == TRUE)
	{
		exit_code = appdata->exit_code;
		free_env(appdata->env);
		free_envp_array(appdata->envp);
		free_memory(appdata);
		exit(exit_code);
	}
	update_exit_code(appdata);
	free_memory(appdata);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_appdata	appdata;

	(void)argc;
	(void)argv;
	if (initialization(&appdata, envp) == FAILURE)
		return (FAILURE);
	while (1)
	{
		input = readline("minishell: ");
		if (!input)
			break ;
		save_history(input);
		run_parsing(input, &appdata);
		// print_tokens(&appdata);
		free(input);
		run_lexer(&appdata);
		// print_lists(&appdata);
		if (appdata.exit_code != 2)
			start_execution(&appdata);
		new_cycle_preparation(&appdata);
	}
	return (appdata.exit_code);
}
