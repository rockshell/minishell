/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:15:13 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/09/03 14:43:47 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <signal.h>
# include <termios.h>
# include <term.h>
# include <unistd.h>
# include "libft.h"

# define ALLOC_ERROR "Error! The program failed to allocate memory!\n"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	int		argc;
	char	*original;
	char	*cmd;
	char	**argv;
}	t_token;

typedef struct s_appdata
{
	t_token	*tokens;
	t_env	*env;
	char **tokens;
}	t_appdata;

//enviromentals
int	create_node(t_env **env, char *current_env);
int	initial_parsing(char *input, t_appdata *appdata);

//utils 
int ft_isspace(char c);
size_t	handle_len_quotes(char *input, size_t i);
char	*handle_num_quotes(char *input);
void	free_tokens(char **tokens);

//parsing
void	initial_parsing(char *input, t_appdata *appdata);

#endif