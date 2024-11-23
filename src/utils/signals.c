/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:38:36 by akulikov          #+#    #+#             */
/*   Updated: 2024/11/12 19:28:30 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int signum)
{
	(void) signum;
	printf("\nminishell: ");
}

void	sigquit_handler(int signum)
{
	(void) signum;
	printf("minishell: ");
	// printf("\n\033[1A\033[6Cexit\n");
	// exit(EXIT_SUCCESS);
}