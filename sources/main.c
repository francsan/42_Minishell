/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:19:19 by francisco         #+#    #+#             */
/*   Updated: 2023/08/08 16:19:35 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// JUST FOR TESTING
void	print_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		printf("%s%s%s\n", YELLOW, arr[i], DEFAULT);
}

void	print_tokens(t_data **d, char **tokens)
{
	int	i;

	i = -1;
	while (tokens[++i])
	{
		printf("%s%s%s\n", BLUE, (*d)->tokens[i].token, DEFAULT);
		printf("Pipe: %i\n", (*d)->tokens[i].f_pipe);
		printf("Input Redirection: %i\n", (*d)->tokens[i].f_r_in);
		printf("Output Redirection: %i\n", (*d)->tokens[i].f_r_out);
		printf("File: %i\n", (*d)->tokens[i].f_file);
		printf("Single Quotes: %i\n", (*d)->tokens[i].f_singlequotes);
		printf("Double Quotes: %i\n", (*d)->tokens[i].f_doublequotes);
		printf("Command: %i\n", (*d)->tokens[i].f_command);
		printf("Flag: %i\n", (*d)->tokens[i].f_flag);
		printf("Builtin: %i\n", (*d)->tokens[i].f_builtin);
		int	j = -1;
		while (++j < 7)
			printf("Builtins[%i] = %i\n", j, (*d)->tokens->f_builtins[j]);
		printf("Dollar Variable: %i\n", (*d)->tokens[i].f_dollarsign);
		printf("\n");
	}
}
// JUST FOR TESTING

int	g_exitvalue;

static void	free_stuff(t_data **d, char **line, char **buffer)
{
	free((*d)->tokens);
	free(*line);
	free(*buffer);
}

static int	run_shell(t_data **d)
{
	char	*buffer;
	char	*line;
	int		i;

	buffer = readline(PROMPT);
	line = sort_line(d, buffer);
	ignore_signal_i();
	if_ctrl_d(d, buffer, line, env_func());
	if (buffer && ft_strlen(buffer) > 0)
		add_history(buffer);
	if (parse_command(d, line))
	{
		free(line);
		free(buffer);
		return (1);
	}
	ignore_signal();
	if ((*d)->num_commands != 0 || (*d)->flag_builtin != 0)
	{
		i = -1;
		while ((*d)->tokens[++i].token)
			free((*d)->tokens[i].token);
	}
	free_stuff(d, &line, &buffer);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*d;

	(void) argc;
	(void) argv;
	d = ft_calloc(1, sizeof(t_data));
	d->env = ft_strarr_copy(envp);
	env_func()->env = env_create(envp);
	get_paths(&d);
	ignore_signal();
	while (1)
	{
		if (run_shell(&d))
		{
			ft_strarr_free(d->paths);
			ft_strarr_free(d->env);
			ft_strarr_free(env_func()->env);
			free(d);
			return (g_exitvalue);
		}
	}
	free_all(&d, env_func());
	return (g_exitvalue);
}
