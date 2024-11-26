/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:17:26 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/11/26 18:58:05 by vkinsfat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_sig_received;

void	save_history(char *cmd)
{
	if (cmd && *cmd)
		add_history(cmd);
}

void	update_exit_code(t_appdata *appdata)
{
	int	last_index;
	int	exit_code;

	last_index = appdata->lists_num - 1;
	if (appdata->lists && appdata->lists_num > 0)
	{
		exit_code = appdata->lists[last_index].exec_data->status;
		free(appdata->exit_status->value);
		appdata->exit_status->value = ft_itoa(exit_code);
		appdata->exit_code = 0;
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
		free_env(appdata->exit_status);
		free_char_array(appdata->envp);
		free_memory(appdata);
		exit(exit_code);
	}
	update_exit_code(appdata);
	free_memory(appdata);
}

char	*get_the_input(t_appdata *appdata)
{
	char	*input;

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	input = readline("minishell: ");
	if (input == NULL)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		appdata->should_exit = TRUE;
		new_cycle_preparation(appdata);
	}
	if (input)
		save_history(input);
	return (input);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_appdata	appdata;

	(void)argc;
	(void)argv;
	g_sig_received = 0;
	if (initialization(&appdata, envp) == FAILURE)
		return (FAILURE);
	while (1)
	{
		input = get_the_input(&appdata);
		run_parsing(input, &appdata);
		free(input);
		run_lexer(&appdata);
		if (appdata.exit_code == 0 && appdata.first_token)
			start_execution(&appdata);
		new_cycle_preparation(&appdata);
	}
	return (appdata.exit_code);
}
// print_tokens(&appdata);
