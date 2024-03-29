/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 14:26:23 by francisco         #+#    #+#             */
/*   Updated: 2023/08/08 16:13:40 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	get_paths(t_data **d)
{
	int	i;

	i = -1;
	while ((*d)->env[++i])
	{
		if (ft_strncmp((*d)->env[i], "PATH=", 5) == 0)
		{
			(*d)->paths = ft_split(&(*d)->env[i][5], ':');
			break ;
		}
	}
}

int	get_pipe_num(char *buffer)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (buffer[i])
	{
		if (buffer[i] == '"')
		{
			while (buffer[i] != '"')
				i++;
		}
		else if (buffer[i] == '\'')
		{
			while (buffer[i] != '\'')
				i++;
		}
		else if (buffer[i] == '|')
			j++;
		i++;
	}
	return (j);
}

void	space_pipes(char *buffer, char **line, int *i, int *j)
{
	if (buffer[(*i)] == '|')
	{
		(*line)[(*j)] = ' ';
		(*line)[(*j) + 1] = buffer[(*i)];
		(*line)[(*j) + 2] = ' ';
		(*i)++;
		(*j) += 3;
	}
	else
	{
		(*line)[(*j)] = buffer[(*i)];
		(*i)++;
		(*j)++;
	}
}

char	*sort_line(t_data **d, char *buffer)
{
	char	*line;
	int		i;
	int		j;

	if (!buffer)
		return (NULL);
	i = get_pipe_num(buffer);
	(*d)->num_pipes = i;
	line = ft_calloc(ft_strlen(buffer) + (i * 2) + 1, sizeof(char));
	i = 0;
	j = 0;
	while (buffer[i])
		space_pipes(buffer, &line, &i, &j);
	return (line);
}

void	free_all(t_data **d, t_env *env)
{
	int	i;

	i = 0;
	if ((*d)->tokens)
	{
		while ((*d)->tokens[i].token)
		{
			free((*d)->tokens[i].token);
			i++;
		}
		free((*d)->tokens);
	}
	ft_strarr_free((*d)->paths);
	ft_strarr_free((*d)->env);
	ft_strarr_free(env->env);
	free(d);
}
