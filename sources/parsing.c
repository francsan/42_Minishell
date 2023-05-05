/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 19:52:37 by francisco         #+#    #+#             */
/*   Updated: 2023/05/05 14:26:12 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	fill_tokens_struct(t_data **d, char **tokens)
{
	t_ints	n;

	n.i = 0;
	while (tokens[n.i])
		n.i++;
	(*d)->tokens = ft_calloc(n.i, sizeof(t_token));
	n.i = -1;
	while (tokens[++n.i])
		(*d)->tokens[n.i].token = ft_strdup(tokens[n.i]);
}

void	sort_tokens(t_data **d, char **tokens)
{
	t_ints	n;

	fill_tokens_struct(d, tokens);
	n.i = -1;
	while (tokens[++n.i])
	{
		n.j = 0;
		while ((*d)->tokens[n.i].token[n.j])
		{
			run_checks(d, &n);
			run_checks_quotes(d, &n);
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
		handle_single_cmd(&d);
	else if (d->num_commands >= 2)
		handle_multiple_cmds(&d);
	ft_strarr_free(tokens);
	return (0);
}
