/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 14:16:36 by francisco         #+#    #+#             */
/*   Updated: 2023/08/08 16:17:31 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	try_paths(t_data **d, t_ints *n, int i)
{
	char	*temp1;
	char	*temp2;

	temp1 = ft_strjoin((*d)->paths[i], "/");
	temp2 = ft_strjoin(temp1, (*d)->tokens[n->i].token);
	if (access(temp2, F_OK) == 0 && (*d)->tokens[n->i].token[0] != '.')
	{
		free((*d)->tokens[n->i].token);
		(*d)->tokens[n->i].token = ft_strdup(temp2);
		free(temp1);
		free(temp2);
		return (1);
	}
	free(temp1);
	free(temp2);
	return (0);
}

int	check_for_command(t_data **d, t_ints *n)
{
	int		i;

	i = -1;
	while ((*d)->tokens[n->i].token[++i])
	{
		if (((*d)->tokens[n->i].token[0] == '/' \
			|| (*d)->tokens[n->i].token[0] == '.') \
			&& access((*d)->tokens[n->i].token, F_OK) == 0)
			return (0);
	}
	i = -1;
	while ((*d)->paths[++i])
		if (try_paths(d, n, i))
			return (0);
	return (1);
}

void	run_checks(t_data **d, t_ints *n)
{
	if (check_for_command(d, n) == 0)
	{
		(*d)->tokens[n->i].f_command = 1;
		(*d)->num_commands++;
	}
	while ((*d)->tokens[n->i].token[n->j] \
		&& (*d)->tokens[n->i].token[n->j] != '"' \
		&& (*d)->tokens[n->i].token[n->j] != '\'')
	{
		if ((*d)->tokens[n->i].token[n->j] == '|')
			(*d)->tokens[n->i].f_pipe = 1;
		if ((*d)->tokens[n->i].token[n->j] == '<')
			(*d)->tokens[n->i].f_r_in = 1;
		if ((*d)->tokens[n->i].token[n->j] == '>')
			(*d)->tokens[n->i].f_r_out = 1;
		if ((*d)->tokens[n->i].token[n->j] == '-')
			(*d)->tokens[n->i].f_flag = 1;
		if ((*d)->tokens[n->i].token[n->j] == '$')
			(*d)->tokens[n->i].f_dollarsign = 1;
		n->j++;
	}
}

void	run_checks_quotes(t_data **d, t_ints *n)
{
	if ((*d)->tokens[n->i].token[n->j] == '"')
	{
		(*d)->tokens[n->i].f_doublequotes = 1;
		n->j++;
		while ((*d)->tokens[n->i].token[n->j] != '"')
		{
			if ((*d)->tokens[n->i].token[n->j] == '$')
				(*d)->tokens[n->i].f_dollarsign = 1;
			n->j++;
		}
		n->j++;
	}
	else if ((*d)->tokens[n->i].token[n->j] == '\'')
	{
		(*d)->tokens[n->i].f_singlequotes = 1;
		n->j++;
		while ((*d)->tokens[n->i].token[n->j] != '\'')
			n->j++;
		n->j++;
	}
}

void	cmd_not_found(t_data **d, char **tokens)
{
	int	i;

	if (tokens[0][0] == '\0')
	{
		i = -1;
		while ((*d)->tokens[++i].token)
			free((*d)->tokens[i].token);
		return ;
	}
	g_exitvalue = 127;
	printf("%s: command not found\n", tokens[0]);
	i = -1;
	while ((*d)->tokens[++i].token)
		free((*d)->tokens[i].token);
}
