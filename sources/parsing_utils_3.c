/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 18:20:26 by francsan          #+#    #+#             */
/*   Updated: 2023/08/29 18:54:32 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	get_variable_2(t_ints *m, char *token, char ***vars)
{
	if (token[m->i] == '$')
	{
		m->i++;
		m->j = 0;
		m->k = m->i;
		while (token[m->i] && check_char(token[m->i]) == 0)
		{
			m->i++;
			m->j++;
		}
		(*vars)[m->l] = ft_calloc(m->j + 1, sizeof(char));
		m->j = 0;
		while (token[m->k] && check_char(token[m->k]) == 0)
			(*vars)[m->l][m->j++] = token[m->k++];
		m->l++;
	}
	else
		m->i++;
}

void	get_values_2(t_ints *m, char ***vars_values, char **vars, char **env)
{
	m->i = -1;
	while (env[++m->i])
	{
		if (ft_strncmp(env[m->i], vars[m->j], ft_strlen(vars[m->j])) == 0)
		{
			(*vars_values)[m->j] = \
			ft_strdup(&env[m->i][ft_strlen(vars[m->j]) + 1]);
			m->j++;
			return ;
		}
	}
	if (!env[m->i] && !(*vars_values)[m->j])
	{
		(*vars_values)[m->j] = ft_strdup("");
		m->j++;
		return ;
	}
}

char	*expand_variable_2(t_data **d, t_ints *n, \
		char **vars, char **vars_values)
{
	t_ints	m;
	char	*token;

	token = ft_calloc(get_calloc_size(d, n, vars, vars_values), sizeof(char));
	m.i = 0;
	m.j = 0;
	m.k = 0;
	while ((*d)->tokens[n->i].token[m.i])
	{
		if ((*d)->tokens[n->i].token[m.i] != '$')
			token[m.j++] = (*d)->tokens[n->i].token[m.i++];
		else
		{
			m.i += ft_strlen(vars[m.k]) + 1;
			m.l = 0;
			while (vars_values[m.k][m.l])
				token[m.j++] = vars_values[m.k][m.l++];
			m.k++;
		}
	}
	return (token);
}

int	check_for_empty_pipe(t_data **d)
{
	t_ints	n;

	n.i = 0;
	while ((*d)->tokens[n.i].token && (*d)->tokens[n.i + 1].token)
	{
		if ((*d)->tokens[n.i].f_pipe == 1 && (*d)->tokens[n.i + 1].f_pipe == 1)
		{
			// n.j = 0;
			// if ((*d)->tokens)
			// {
			// 	while ((*d)->tokens[n.j].token)
			// 	{
			// 		free((*d)->tokens[n.j].token);
			// 		n.j++;
			// 	}
			// 	free((*d)->tokens);
			// }
			printf("Minishell: syntax error near unexpected token `||'\n");
			return (1);
		}
		n.i++;
	}
	return (0);
}
