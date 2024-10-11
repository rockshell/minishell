/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:52:48 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/10 14:48:55 by vitakinsfat      ###   ########.fr       */
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
# define CD_ONE_ARG_ERROR "Error! No absolute or relative path specified!\n"
# define CD_TOO_MANY_ARG_ERROR "Error! Too many arguments!"
# define EXIT_TOO_MANY_ARG_ERROR "minishell: exit: too many arguments\n"
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
	int		is_builtin;
	char	*original;
	char	*cmd;
	char	**argv;
	char	*filename;
	char	*delim;
}	t_cmd_token;

typedef struct s_exec_data
{
	int		infile;
	int		outfile;
	int		status;
	int		pipe_counter;
	int		input_redirection_num;
	int		output_redirection_num;
	int		**fd;
	pid_t	*processes;
}	t_exec_data;

typedef struct s_appdata
{
	int			exit_code;
	int			num_of_input_strings;
	int			cmd_tokens_num;
	int			srv_tokens_num;
	char		**input_strings;
	char		**envp_array;
	t_exec_data	*exec_data;
	t_cmd_token	*cmd_tokens;
	t_srv_token	*srv_tokens;
	t_env		*env_var;
}	t_appdata;

//enviromentals
int			initialize_env_var(t_appdata *appdata, char **envp);
int			create_env_var_node(t_env **env, char *current_env);
char		*ft_get_env(t_env *env, char *key);
void		free_and_renew_env_value(t_env *env, char *key, char *new_value);
void		print_env(t_env *env, char *key);
char		*get_key(char *current_env);
char		*get_value(char *current_env);

//utils 
void		free_memory(t_appdata *appdata);
void		error_rising(t_appdata *appdata);

//built-in
int			ft_cd(t_cmd_token *token, t_env *env);
int			ft_echo(t_cmd_token *token);
int			ft_env(t_env *env);
int			ft_exit(t_appdata *appdata, t_cmd_token *token);
int			ft_export(t_cmd_token *token, t_env *env);
int			ft_pwd(void);
int			ft_unset(t_cmd_token *token, t_env *env);

//built-in utils
int			is_valid_digit(char *str);
long long	ft_atoll(char *str);
int			ft_strcmp(char *first_str, char *second_str);
int			is_valid_var(char *argument);
int			is_in_var(t_env *env, char *argument);
char		**sort_an_array(char **array, int len);
char		**get_an_array_of_keys(t_env *env, int len);
int			get_length_of_env(t_env *env);

//parsing - utils
int			ft_isspace(char c);
char		*handle_num_quotes(char *input);
void		free_tokens(char **tokens);
size_t		handle_len_quotes(char *input, size_t i);
int			initial_parsing(char *input, t_appdata *appdata);
int			get_type_of_token(char *command);
int			count_service_tokens(t_appdata *appdata, char **input_strings);
int			count_command_tokens(t_appdata *appdata, char **input_strings);
void		fill_service_tokens(t_appdata *appdata);
void		fill_command_tokens(t_appdata *appdata);

//execution utils
int			open_files(t_appdata *appdata, int is_in);
void		rwr_heredoc(t_appdata *appdata, char *delim);
char		*get_next_line(int fd);
char		*gnl_strjoin(char const *s1, char const *s2);
size_t		gnl_strlen(const char *str);
void		get_number_of_pipe_and_redirection(t_appdata *appdata);
char		*make_path(t_cmd_token token);
void		close_pipes_in_parent(t_appdata *appdata);
void		io_redirection(t_appdata *appdata, int is_infile);
void		close_fds(t_exec_data *exec_data, int current_pipe);
int			check_if_builtin(t_cmd_token *token);

//execution
void		start_execution(t_appdata *appdata);
void		run_lexer(t_appdata *appdata);

//children
void		first_child(t_appdata *appdata);
void		last_child(t_appdata *appdata, int i);
void		mid_child(t_appdata *appdata, int i);
void		only_child(t_appdata *appdata);
#endif