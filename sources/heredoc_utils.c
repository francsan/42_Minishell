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
		m.l = 0;
		m.a = 0;
		while (buffer[m.l] && m.a <= m.i)
		{
			if (buffer[m.l] == '$')
			{
				if (m.a == m.i)
				{
					m.b = m.l;
					while (buffer[m.l] != ' ' && buffer[m.l] != '$' && buffer[m.l])
						m.l++;
				}
				m.a++;
				break ;
			}
			m.l++;
		}
		buffer_vars[m.i] = ft_calloc((m.l - m.b) + 2, sizeof(char *));
		m.i++;
	}
	m.i = 0;
	m.j = 0;
	while (buffer[m.i])
	{
		if (buffer[m.i] == '$')
		{
			m.k = 0;
			m.i++;
			while (buffer[m.i] != ' ' && buffer[m.i] != '$' && buffer[m.i])
				buffer_vars[m.j][m.k++] = buffer[m.i++];
			m.j++;
		}
		else
			m.i++;
	}
	return (buffer_vars);
}

char	**get_env_values(t_ints *n, char **buffer_vars, char **env)
{
	t_ints	m;
	char	**env_values;

	env_values = ft_calloc(n->j + 1, sizeof(char *));
	m.i = 0;
	while (buffer_vars[m.i])
	{
		m.j = 0;
		while (env[m.j])
		{
			if (ft_strncmp(buffer_vars[m.i], env[m.j], ft_strlen(buffer_vars[m.i]) - 1) == 0)
			{
				if (m.i + 1 < n->j)
					env_values[m.i] = ft_strdup(&env[m.j][ft_strlen(buffer_vars[m.i]) + 1]);
				else
					env_values[m.i] = ft_strdup(&env[m.j][ft_strlen(buffer_vars[m.i])]);
				break ;
			}
			m.j++;
		}
		m.i++;
	}
	return (env_values);
}

void	expand_dollar_var(t_ints *n, char **buffer)
{
	char	**buffer_vars;
	char	**env_values;
	char	*tmp;

	buffer_vars = get_buffer_vars(n, *buffer);
	print_array(buffer_vars); // TESTING
	env_values = get_env_values(n, buffer_vars, env_func()->env);
	print_array(env_values); // TESTING
	tmp = ft_strdup(buffer_vars[0]);
	ft_strarr_free(buffer_vars);
	free(*buffer);
	*buffer = ft_strdup(tmp);
	free(tmp);
}
