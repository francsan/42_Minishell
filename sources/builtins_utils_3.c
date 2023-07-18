/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 00:26:04 by francisco         #+#    #+#             */
/*   Updated: 2023/07/18 18:51:04 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	problem_solver(char *str, char **temp)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	*temp = ft_calloc(i + 1, sizeof(char));
	i = 0;
	while (str[i] && str[i] != '=')
	{
		(*temp)[i] = str[i];
		i++;
	}
}

int	check_var_exist(char *str, char **env)
{
	int		i;
	char	*temp;
	char	*temp2;

	problem_solver(str, &temp);
	i = -1;
	while (env[++i])
	{
		problem_solver(env[i], &temp2);
		if (ft_strlen(temp) >= ft_strlen(temp2))
		{
			if (ft_strncmp(temp, temp2, ft_strlen(temp)) == 0)
			{
				free(temp);
				free(temp2);
				return (i);
			}
		}
		else
		{
			if (ft_strncmp(temp, temp2, ft_strlen(temp2)) == 0)
			{
				free(temp);
				free(temp2);
				return (i);
			}
		}
		free(temp2);
	}
	free(temp);
	return (-1);
}

int	ft_len(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
		;
	return (i);
}

int	handle_new_var(char **tokens, t_env *env, int i)
{
	char	**tmp;
	int		j;

	j = -1;
	tmp = env->env;
	env->env = ft_calloc((ft_len(env->env) + 2), sizeof(char *));
	if (!env->env)
	{
		env->env = tmp;
		return (-1);
	}
	while (tmp[++j])
		env->env[j] = tmp[j];
	free(tmp);
	env->env[j] = ft_strdup(tokens[i]);
	return (0);
}
