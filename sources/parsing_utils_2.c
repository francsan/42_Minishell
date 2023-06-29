/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 20:03:53 by francisco         #+#    #+#             */
/*   Updated: 2023/06/29 18:36:34 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	check_char(char c)
{
	if (c >= 65 && c <= 90)
		return (0);
	if (c >= 97 && c <= 122)
		return (0);
	if (c >= 48 && c <= 57)
		return (0);
	if (c == 95)
		return (0);
	return (1);
}

char	**get_variable(t_data **d, t_ints *n)
{
	t_ints	m;
	char	*token;
	char	**vars;

	token = ft_strdup((*d)->tokens[n->i].token);
	m.i = -1;
	m.l = 0;
	while (token[++m.i])
		if (token[m.i] == '$')
			m.l++;
	vars = ft_calloc(m.l + 1, sizeof(char *));
	m.i = 0;
	m.l = 0;
	while (token[m.i])
		get_variable_2(&m, token, &vars);
	free(token);
	return (vars);
}

char	**get_values(char **vars)
{
	char	**env;
	char	**vars_values;
	t_ints	m;

	env = ft_strarr_copy(env_func()->env);
	m.k = 0;
	while (vars[m.k])
		m.k++;
	vars_values = ft_calloc(m.k + 1, sizeof(char *));
	m.j = 0;
	while (vars[m.j])
		get_values_2(&m, &vars_values, vars, env);
	ft_strarr_free(env);
	return (vars_values);
}

int	get_calloc_size(t_data **d, t_ints *n, char **vars, char **vars_values)
{
	t_ints	m;

	m.i = 0;
	m.j = 0;
	while (vars_values[m.i])
	{
		m.j += ft_strlen(vars_values[m.i]);
		m.i++;
	}
	m.i = 0;
	m.k = 0;
	while (vars[m.i])
	{
		m.k += ft_strlen(vars[m.i]);
		m.i++;
	}
	m.l = ft_strlen((*d)->tokens[n->i].token);
	return (m.l - m.k + m.j + 1);
}

void	expand_variable(t_data **d, t_ints *n)
{
	char	**vars;
	char	**vars_values;
	char	*token;

	if (ft_strncmp((*d)->tokens[n->i].token, "$?", 2) == 0)
	{
		free((*d)->tokens[n->i].token);
		(*d)->tokens[n->i].token = ft_itoa_2(g_exitvalue);
		return ;
	}
	vars = get_variable(d, n);
	vars_values = get_values(vars);
	token = expand_variable_2(d, n, vars, vars_values);
	free((*d)->tokens[n->i].token);
	(*d)->tokens[n->i].token = ft_strdup(token);
	free(token);
	ft_strarr_free(vars_values);
	ft_strarr_free(vars);
}
