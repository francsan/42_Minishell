/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:19:19 by francisco         #+#    #+#             */
/*   Updated: 2023/05/05 14:30:06 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// JUST FOR TESTING
void	print_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		printf("%s\n", arr[i]);
}

void	print_tokens(t_data **d, char **tokens)
{
	int	i;

	i = -1;
	while (tokens[++i])
	{
		printf("%s%s%s\n", BLUE, (*d)->tokens[i].token, DEFAULT);
		printf("Pipe: %i\n", (*d)->tokens[i].f_pipe);
		printf("Input Redirection: %i\n", (*d)->tokens[i].f_redir_input);
		printf("Output Redirection: %i\n", (*d)->tokens[i].f_redir_output);
		printf("File: %i\n", (*d)->tokens[i].f_file);
		printf("Single Quotes: %i\n", (*d)->tokens[i].f_singlequotes);
		printf("Double Quotes: %i\n", (*d)->tokens[i].f_doublequotes);
		printf("Command: %i\n", (*d)->tokens[i].f_command);
		printf("Flag: %i\n\n", (*d)->tokens[i].f_flag);
	}
}
// JUST FOR TESTING

int	main(int argc, char **argv, char **envp)
{
	t_data	*d;
	char	*line;

	(void)argc;
	(void)argv;
	d = ft_calloc(1, sizeof(t_data));
	d->env = ft_strarr_copy(envp);
	get_paths(&d);
	while (1)
	{
		line = readline(PROMPT);
		if (line && ft_strlen(line) > 0)
			add_history(line);
		if (parse_command(d, line))
			break ;
		free(line);
	}
	free_all(&d);
	free(line);
}
