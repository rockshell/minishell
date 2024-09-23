/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:15:13 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/09/23 15:52:59 by vitakinsfat      ###   ########.fr       */
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
# define BUFFER_SIZE 42

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_srv_token
{
	int		input_id;
	int		output_id;
	char	*original;
	int		type;
}	t_srv_token;

typedef struct s_cmd_token
{
	int		id;
	int		argc;
	char	*original;
	char	*cmd;
	char	**argv;
	char	*filename;
	char	*delim;
}	t_cmd_token;

typedef	struct s_exec_data
{
	int		infile;
	int		outfile;
	int		status;
	int		pipe_counter;
	int     redirect_in_counter;
	int		redirect_out_counter;
	int		**fd;
	pid_t	*processes;
}	t_exec_data;


typedef struct s_appdata
{
	int		num_of_input_strings;
	int     cmd_tokens_num;
	int		srv_tokens_num;
	char 	**input_strings;
	t_exec_data	*exec_data;
	t_cmd_token	*cmd_tokens;
	t_srv_token *srv_tokens;
	t_env	*env;
}	t_appdata;

//enviromentals
int	create_node(t_env **env, char *current_env);

//utils 
void free_memory(t_appdata *appdata);
void error_rising(t_appdata *appdata);


//parsing - utils
int 	ft_isspace(char c);
char	*handle_num_quotes(char *input);
void	free_tokens(char **tokens);
size_t	handle_len_quotes(char *input, size_t i);
void	initial_parsing(char *input, t_appdata *appdata);
int		get_type_of_token(char *command);

//execution utils
int	open_files(t_appdata *appdata, int is_in);
void	rwr_heredoc(t_appdata *appdata, char *delim);
char	*get_next_line(int fd);
char	*gnl_strjoin(char const *s1, char const *s2);
size_t	gnl_strlen(const char *str);
void	count_service_tokens(t_appdata *appdata);

//children
void	first_child(t_appdata *appdata);
void	last_child(t_appdata *appdata, int i);
void	mid_child(t_appdata *appdata, int i);
#endif