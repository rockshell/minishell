/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:15:13 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/10/24 15:56:21 by vitakinsfat      ###   ########.fr       */
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
# include <stdbool.h>
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
# define BAD_INPUT "Syntax error somewhere. \n"
# define BUFFER_SIZE 42
# define CD_ONE_ARG_ERROR "minishell: cd: no absolute or relative path specified\n"
# define CD_TOO_MANY_ARG_ERROR "minishell: cd: too many arguments\n"
# define EXIT_TOO_MANY_ARG_ERROR "minishell: exit: too many arguments\n"

# define WORD 0
# define ARGUMENT 1
# define PIPE 2
# define STDIN 3
# define STDOUT 4
# define HEREDOC 5
# define APPEND 6
# define LOGICAL_AND 7
# define LOGICAL_OR 8
# define SINGLE_QUOTE 9
# define DOUBLE_QUOTE 10

# define SUCCESS 0
# define FAILURE 1
# define COMMAND_MISUSE 2
# define COMMAND_NOT_FOUND 127

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token	t_token;
/**
 *
 * This structure stores the necessary information for each token parsed
 * from the user input in the minishell project, including its position,
 * content, and type. It can also be part of a linked list for sequential
 * token processing.
 * 
 * @var pos The position of the token in the command line.
 * @var content The actual string value of the token (e.g., command, argument).
 * @var type The type of the token (e.g., COMMAND, ARGUMENT, OPERATOR).
 * @var next Pointer to the next token in the list (for linked list structure).
 */
typedef struct s_token
{
	int		pos;
	int		type;
	int		is_parsed;
	char	*value;
	t_token	*prev;
	t_token	*next;
}	t_token;

typedef struct s_cmd
{
	int		argc; //num of things in argv
	int		is_builtin; //TODO
	int		input_redir_type;
	int		output_redir_type;
	int		is_pipe_after;
	int		is_pipe_before;
	char	**argv; //no redirects, command + args
	char	*infile_name;
	char	*outfile_name;
	char	*delim; //for HEREDOC use ONLY
}	t_cmd;

typedef struct s_exec_data
{
	int		infile;
	int		outfile;
	int		status;
	int		**fd;
	pid_t	*processes;
}	t_exec_data;

typedef struct s_list
{
	int			and_after;
	int			or_after;
	int			end_after;
	int			size;
	t_cmd		*cmd;
	t_exec_data	*exec_data; //for Vita's use
}	t_list;

typedef struct s_appdata
{
	int		tokens_num;
	int		lists_num;
	int		exit_code;
	int		should_exit;
	char	**envp;
	t_token	*tokens;
	t_token	*first_token;
	t_list	*lists;
	t_env	*env;
}	t_appdata;

//built-in
int			ft_cd(t_cmd *cmd, t_env *env);
int			ft_echo(t_cmd *cmd);
int			ft_env(t_env *env);
int			ft_exit(t_appdata *appdata, t_cmd *cmd);
int			ft_export(t_cmd *cmd, t_env *env);
int			ft_pwd(void);
int			ft_unset(t_cmd *cmd, t_env *env);

//built-in utils
char		**get_an_array_of_keys(t_env *env, int len);
char		**sort_an_array(char **array, int len);
int			ft_strcmp(char *first_str, char *second_str);
int			get_length_of_env(t_env *env);
int			is_in_var(t_env *env, char *argument);
int			is_valid_digit(char *str);
int			is_valid_var(char *argument);
long long	ft_atoll(char *str);

//enviromentals
int			create_env_node(t_env **env, char *current_env);
int			initialize_env_var(t_appdata *appdata, char **envp);

//environmentals utils
char		*ft_get_env(t_env *env, char *key);
char		*get_key(char *current_env);
char		*get_value(char *current_env);
void		free_and_renew_env_value(t_env *env, char *key, char *new_value);
void		print_env(t_env *env, char *key);

//execution
void		run_lexer(t_appdata *appdata);
void		start_execution(t_appdata *appdata);
void		init_exec_data(t_list *list);
void		execute_single(t_appdata *appdata, t_list *list);

//execution utils
char		*get_next_line(int fd);
char		*gnl_strjoin(char const *s1, char const *s2);
char		*make_path(t_cmd *cmd);
int			check_if_builtin(t_cmd cmd);
int			execute_a_builtin(t_appdata *appdata, t_cmd *cmd);
int			open_files(t_list *list, int is_input);
size_t		gnl_strlen(const char *str);
void		close_pipes_in_parent(t_list *list);
void		close_fds(t_list *list, int current_pipe);
void		io_redirection(t_appdata *appdata, t_list *list, int is_infile);
void		redirect_only_child(t_appdata *appdata, t_list *list);
void		rwr_heredoc(t_appdata *appdata, t_list *list, char *delim);

//children
void		first_child(t_appdata *appdata, t_list *list);
void		last_child(t_appdata *appdata, t_list *list, int i);
void		mid_child(t_appdata *appdata, t_list *list, int i);
void		only_child(t_appdata *appdata, t_list *list);

//utils 
void		free_memory(t_appdata *appdata);
void		free_env(t_env *env);
void		error_rising(t_appdata *appdata);
void		free_exec_data(t_list *list);
void		free_lists(t_list *list);

//parsing - utils
char		*handle_num_quotes(char *input);
int			ft_isspace(char c);
int			run_parsing(char *input, t_appdata *appdata);
int			count_lists(t_appdata *appdata);
int			is_cmd_end(t_token *token);
int			is_list_end(t_token *token);
size_t		handle_len_quotes(char *input, size_t i);
void		init_token(int i, t_token *current);
void		set_pipes_in_cmd(t_cmd *cmd, int pipe_flag, t_token *last);
void		set_redirections_in_cmd(t_cmd *cmd, t_token *current);
void		set_the_command_itself(t_cmd *cmd, t_token *first);

#endif
