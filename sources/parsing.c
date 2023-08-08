/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 19:52:37 by francisco         #+#    #+#             */
/*   Updated: 2023/08/08 17:41:55 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	fill_tokens_struct(t_data **d, char **tokens)
{
	t_ints	n;

	n.i = 0;
	while (tokens[n.i])
		n.i++;
	(*d)->tokens = ft_calloc(n.i + 1, sizeof(t_token));
	n.i = -1;
	while (tokens[++n.i])
		(*d)->tokens[n.i].token = ft_strdup(tokens[n.i]);
}

int	fill_flags(t_data **d, t_ints *n, int builtin)
{
	(*d)->tokens[n->i].f_builtin = 1;
	(*d)->tokens[n->i].f_builtins[builtin] = 1;
	(*d)->num_commands++;
	return (1);
}

int	check_for_builtin(t_data **d, t_ints *n)
{
	if (ft_strncmp((*d)->tokens[n->i].token, "cd", 2) == 0)
		return (fill_flags(d, n, 1));
	else if (ft_strncmp((*d)->tokens[n->i].token, "export", 6) == 0)
		return (fill_flags(d, n, 3));
	else if (ft_strncmp((*d)->tokens[n->i].token, "unset", 5) == 0)
		return (fill_flags(d, n, 4));
	else if (ft_strncmp((*d)->tokens[n->i].token, "exit", 4) == 0)
		return (fill_flags(d, n, 6));
	return (0);
}

void	sort_tokens(t_data **d, char **tokens)
{
	t_ints	n;

	fill_tokens_struct(d, tokens);
	n.i = -1;
	while (tokens[++n.i])
	{
		if (check_for_builtin(d, &n))
			(*d)->flag_builtin = 1;
		else
		{
			n.j = 0;
			while ((*d)->tokens[n.i].token[n.j])
			{
				if ((*d)->tokens[n.i].token[n.j] == '\t')
					(*d)->tokens[n.i].token[n.j] = '\0';
				run_checks(d, &n);
				run_checks_quotes(d, &n);
				if ((*d)->tokens[n.i].token[n.j])
					run_checks(d, &n);
			}
		}
		if ((*d)->tokens[n.i].f_dollarsign == 1)
			expand_variable(d, &n);
	}
}

int	parse_command(t_data **d, char *line)
{
	char	**tokens;

	if (!line)
		return (1);
	tokens = ft_minishell_split(line);
	if (!tokens)
		return (1);
	(*d)->num_commands = 0;
	(*d)->flag_builtin = 0;
	sort_tokens(d, tokens);
	if ((*d)->num_commands == 0 && tokens[0])
		cmd_not_found(d, tokens);
	else if ((*d)->num_commands == 1 && (*d)->flag_builtin == 1)
		handle_builtin_cmd(d, &tokens);
	else if ((*d)->num_commands == 1 && (*d)->flag_builtin == 0 \
		&& (*d)->num_pipes == 0)
		handle_single_cmd(d);
	else if ((*d)->num_pipes >= 1)
		handle_multiple_cmds(d);
	ft_strarr_free(tokens);
	return (0);
}
