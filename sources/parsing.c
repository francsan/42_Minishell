/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 19:52:37 by francisco         #+#    #+#             */
/*   Updated: 2023/04/27 16:50:49 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// JUST FOR TESTING
// static void	print_tokens(t_data **d, char **tokens)
// {
// 	int	i;

// 	i = -1;
// 	while (tokens[++i])
// 	{
// 		printf("%s%s%s\n", BLUE, (*d)->tokens[i].token, DEFAULT);
// 		printf("Pipe: %i\n", (*d)->tokens[i].f_pipe);
// 		printf("Redirection: %i\n", (*d)->tokens[i].f_redirection);
// 		printf("File %i\n", (*d)->tokens[i].f_file);
// 		printf("Single Quotes: %i\n", (*d)->tokens[i].f_singlequotes);
// 		printf("Double Quotes: %i\n", (*d)->tokens[i].f_doublequotes);
// 		printf("Command: %i\n", (*d)->tokens[i].f_command);
// 		printf("Flag: %i\n\n", (*d)->tokens[i].f_flag);
// 	}
// }

void	fill_tokens_struct(t_data **d, char **tokens)
{
	t_iterators	n;

	n.i = 0;
	while (tokens[n.i])
		n.i++;
	(*d)->tokens = ft_calloc(n.i, sizeof(t_token));
	n.i = -1;
	while (tokens[++n.i])
		(*d)->tokens[n.i].token = ft_strdup(tokens[n.i]);
}

int	check_for_command(t_data **d, t_iterators *n)
{
	char	*temp1;
	char	*temp2;
	int		i;

	i = -1;
	while ((*d)->tokens[n->i].token[++i])
	{
		if ((*d)->tokens[n->i].token[i] == '/'
			&& access((*d)->tokens[n->i].token, F_OK) == 0)
			return (0);
	}
	i = -1;
	while ((*d)->paths[++i])
	{
		temp1 = ft_strjoin((*d)->paths[i], "/");
		temp2 = ft_strjoin(temp1, (*d)->tokens[n->i].token);
		if (access(temp2, F_OK) == 0)
		{
			free((*d)->tokens[n->i].token);
			(*d)->tokens[n->i].token = ft_strdup(temp2);
			free(temp1);
			free(temp2);
			return (0);
		}
		free(temp1);
		free(temp2);
	}
	return (1);
}

void	run_checks(t_data **d, t_iterators *n)
{
	if (check_for_command(d, n) == 0)
	{
			(*d)->tokens[n->i].f_command = 1;
			(*d)->num_commands++;
	}
	while ((*d)->tokens[n->i].token[n->j]
			&& (*d)->tokens[n->i].token[n->j] != '"' && (*d)->tokens[n->i].token[n->j] != '\'')
	{
		if ((*d)->tokens[n->i].token[n->j] == '|')
			(*d)->tokens[n->i].f_pipe = 1;
		if ((*d)->tokens[n->i].token[n->j] == '<' || (*d)->tokens[n->i].token[n->j] == '>')
			(*d)->tokens[n->i].f_redirection = 1;
		if ((*d)->tokens[n->i].token[n->j] == '-')
			(*d)->tokens[n->i].f_flag = 1;
		n->j++;
	}
}

void	sort_tokens(t_data **d, char **tokens)
{
	t_iterators	n;

	fill_tokens_struct(d, tokens);
	n.i = -1;
	while (tokens[++n.i])
	{
		n.j = 0;
		while ((*d)->tokens[n.i].token[n.j])
		{
			run_checks(d, &n);
			if ((*d)->tokens[n.i].token[n.j] == '"')
			{
				(*d)->tokens[n.i].f_doublequotes = 1;
				n.j++;
				while ((*d)->tokens[n.i].token[n.j] != '"')
					n.j++;
				n.j++;
			}
			else if ((*d)->tokens[n.i].token[n.j] == '\'')
			{
				(*d)->tokens[n.i].f_singlequotes = 1;
				n.j++;
				while ((*d)->tokens[n.i].token[n.j] != '\'')
					n.j++;
				n.j++;
			}
			if ((*d)->tokens[n.i].token[n.j])
				run_checks(d, &n);
		}
	}
}

int	parse_command(t_data *d, char *line)
{
	char	**tokens;

	if (!line)
		return (1);
	tokens = ft_minishell_split(line);
	if (!tokens)
		return (1);
	d->num_commands = 0;
	sort_tokens(&d, tokens);
	if (d->num_commands == 1)
		handle_command(&d); // RUN SINGLE COMMAND
	// else if (d->num_commands >= 2)
	// 	handle_pipes(); // RUN MULTIPLE COMMANDS
	// print_tokens(&d, tokens); // TESTING
	ft_strarr_free(tokens);
	return (0);
}
