/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:19:23 by francisco         #+#    #+#             */
/*   Updated: 2023/05/10 18:56:51 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* libraries */

// readline and printf
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

// malloc and free
# include <stdlib.h>

// write and usleep
# include <unistd.h>

// waitpid
# include <sys/types.h>
# include <sys/wait.h>

// open
# include <fcntl.h>

/* strings */

# define PROMPT "Minishell-> "

/* structs */

typedef struct s_red	t_red;
typedef struct s_cmd	t_cmd;

typedef struct s_red {
	int		fd;
	int		is_two;
	int		output;
	char	*file;
	t_red	*next;
}	t_red;

typedef struct s_cmd {
	int		outfd;
	int		infd;
	t_red	*io;
	t_cmd	*next;
}	t_cmd;

typedef struct t_ints {
	int	i;
	int	j;
	int	k;
}	t_ints;

typedef	struct t_builtins
{
	int	echo;
	int	cd;
	int	pwd;
	int	export;
	int	unset;
	int	env;
	int	exit;
}	t_builtins;


typedef struct t_token {
	char	*token;
	int		f_pipe;
	int		f_redir_input;
	int		f_redir_output;
	int		f_file;
	int		f_singlequotes;
	int		f_doublequotes;
	int		f_envvar;
	int		f_command;
	int		f_flag;
	int		f_builtin;
	int		f_builtins[7];
}	t_token;

typedef struct t_data {
	char	**env;
	char	**paths;
	int		num_commands;
	int		flag_builtin;
	int		old_fd;
	int		fd[2];
	pid_t	*pid;
	t_token	*tokens;
}	t_data;

/* sources */

// main.c TESTING
void	print_array(char **arr);
void	print_tokens(t_data **d, char **tokens);

// command_handling.c
t_cmd   *add_cmd(t_cmd *cmd);
void	handle_command(t_data **d, char **tokens);

char	**get_cmd(t_data **d, t_ints *n);
void	handle_builtin_cmd(t_data **d);
void	handle_single_cmd(t_data **d);
void	run_cmd(t_data **d, t_ints *n, char ***cmds);
void	handle_multiple_cmds(t_data **d);

// minishell_split_utils.c
int		check_quote(t_ints *n, char *line, char quote);
int		check_command(t_ints *n, char *line);
void	get_quote_len(t_ints *n, char *line, char quote);
void	get_quote(t_ints *n, char ***tokens, char *line, char quote);

// minishell_split.c
char	**alloc_tokens_array(char *line);
int		alloc_tokens_strings(char ***tokens, char *line);
void	fill_tokens(char ***tokens, char *line, int token_num);
char	**ft_minishell_split(char *line);

// parsing_utils.c
int		try_paths(t_data **d, t_ints *n, int i);
int		check_for_command(t_data **d, t_ints *n);
void	run_checks(t_data **d, t_ints *n);
void	run_checks_quotes(t_data **d, t_ints *n);

// parsing.c
void	fill_tokens_struct(t_data **d, char **tokens);
int		fill_flags(t_data **d, t_ints *n, int builtin);
int		check_for_builtin(t_data **d, t_ints *n);
void	sort_tokens(t_data **d, char **tokens);
int		parse_command(t_data *d, char *line);

// pipes.c
void	close_pipe(t_data **d);
void	handle_pipes(t_data **d, t_ints *n);

// redirect_utils.c
int		check_redirection(char **tokens, t_cmd *cmd, int i);
int		pass_red(char *tokens, t_cmd *cmd, int i);
t_red	*add_red(char *file, int is_two, int output, t_red **beg);

// redirect.c
int 	redir_prep(t_cmd *cmd);
int 	redir_check(t_cmd *cmd);
int		treat_output(t_red *red, t_cmd *cmd);
int		treat_input(t_red *red, t_cmd *cmd);
int		check_io_dup(t_red *red);
void	rem_ref(t_red *rem);

// utils.c
void	get_paths(t_data **d);
void	free_all(t_data **d);

/* functions */

// ft_calloc.c
void	*ft_memset(void *b, int c, size_t len);
void	*ft_calloc(size_t count, size_t size);

// ft_split.c
int		mem_size(char const *s, char c);
int		word_size(const char *s, char c);
char	**fill_strs(char **strs, const char *s, char c);
char	**ft_split(char const *s, char c);

// ft_strarr_cpy.c
char	**ft_strarr_copy(char **arr);

// ft_strarr_free.c
void	ft_strarr_free(char **array);

// ft_strdup.c
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_strdup(const char *s1);

// ft_strjoin.c
char	*ft_strjoin(char const *s1, char const *s2);

// ft_strlen.c
size_t	ft_strlen(const char *s);

// ft_strncmp.c
int		ft_strncmp(const char *s1, const char *s2, size_t n);

/* colors */

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define BLUE "\033[0;34m"
# define YELLOW "\033[0;33m"
# define CYAN_B "\033[1;96m"
# define DEFAULT "\033[0m"
# define DEFAULT_B "\033[1;0m"

#endif