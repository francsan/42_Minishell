/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 18:35:15 by francsan          #+#    #+#             */
/*   Updated: 2023/08/08 19:57:29 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	check_for_dollar(t_ints *n, char *buffer)
{
	n->i = 0;
	n->j = 0;
	while (buffer[n->i])
	{
		if (buffer[n->i] == '$')
			n->j++;
		n->i++;
	}
	if (n->j > 0)
		return (1);
	else
		return (0);
}

char	**get_buffer_vars(t_ints *n, char *buffer)
{
	t_ints	m;
	char	**buffer_vars;

	buffer_vars = ft_calloc(n->j + 1, sizeof(char *));
	m.i = 0;
	while (m.i < n->j)
	{
		buffer_vars[m.i] = ft_calloc(100, sizeof(char *));
		m.i++;
	}
	m.i = 0;
	m.j = 0;
	while (buffer[m.i])
	{
		if (buffer[m.i] == '$')
		{
			m.k = 0;
			while (buffer[m.i] != ' ' && buffer[m.i])
			{
				buffer_vars[m.j][m.k++] = buffer[m.i++];
			}
			m.j++;
		}
		else
			m.i++;
	}
	return (buffer_vars);
}

void	calloc_size(t_ints *n, char **buffer_vars, char **vars, char **vars_values)
{
	t_ints	m;

	n->array = ft_calloc(n->j + 1, sizeof(int));
	n->k = 0;
	m.i = 0;
	m.j = 0;
	while (buffer_vars[m.i])
	{
		while (vars[m.j])
		{
			if (ft_strncmp(buffer_vars[m.i], vars[m.j], ft_strlen(vars[m.j])) == 0)
			{
				n->k += ft_strlen(vars_values[m.j]) - ft_strlen(vars[m.j]);
				n->array[m.i] = m.j;
				m.i++;
				m.j = 0;
			}
			else
				m.j++;
		}
	}
	m.i = 0;
}

char	*expand_dollar_var_2(t_ints *n, char *buffer, char **vars, char **vars_values)
{
	t_ints	m;
	char	**buffer_vars;
	char	*tmp;

	buffer_vars = get_buffer_vars(n, buffer);
	calloc_size(n, buffer_vars, vars, vars_values);
	n->l = 0;
	while (buffer[n->l])
	{
		if (buffer[n->l] == '$')
		{
			while (buffer[n->l] != ' ')
				n->l++;
		}
		else
		{
			n->k++;
			n->l++;
		}
	}
	tmp = ft_calloc(n->k, sizeof(char));
	n->i = 0;
	n->j = 0;
	m.j = 0;
	m.k = 0;
	while (buffer[n->i])
	{
		if (buffer[n->i] == '$')
		{
			n->i += ft_strlen(vars[n->array[n->j]]);
			m.i = 0;
			while (vars_values[n->j][m.i])
			{
				tmp[m.j] = vars_values[n->j][m.i + ft_strlen(buffer_vars[m.k])];
				m.i++;
				m.j++;
			}
			m.k++;
		}
		else
			tmp[m.j++] = buffer[n->i];
	}
	ft_strarr_free(buffer_vars);
	return (tmp);
}

char	**copy_variables(char **env)
{
	t_ints	n;
	char	**vars;

	n.i = 0;
	while (env[n.i])
		n.i++;
	vars = ft_calloc(n.i + 1, sizeof(char *));
	n.i = -1;
	while (vars[++n.i])
		vars[n.i] = ft_strdup(env[n.i]);
	return (vars);
}

void	expand_dollar_var(t_ints *n, char **buffer)
{
	char	**vars;
	char	**vars_values;
	char	*tmp;

	vars = copy_variables(env_func()->env);
	vars_values = ft_strarr_copy(vars);
	tmp = expand_dollar_var_2(n, *buffer, vars, vars_values);
	free(*buffer);
	*buffer = ft_strdup(tmp);
	ft_strarr_free(vars_values);
	ft_strarr_free(vars);
	free(tmp);
}
