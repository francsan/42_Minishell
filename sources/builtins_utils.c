/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:52:28 by francsan          #+#    #+#             */
/*   Updated: 2023/07/20 17:23:10 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	is_valid(char *str)
{
	int	i;

	i = -1;
	while (str[++i] && str[i] != '=')
	{
		if (i == 0 && !((str[i] >= 'a' && str[i] <= 'z') || \
		(str[i] >= 'A' && str[i] <= 'Z')))
			return (0);
		if (i && !ft_isalnum(str[i]))
			return (0);
	}
	return (1);
}

int	strrlen(char *str, char c)
{
	int	len;

	len = -1;
	if (!str)
		return (0);
	while (str[++len] && str[len] != c)
		;
	return (len + (str[len] == c));
}

int	env_set(char **tokens, t_env *env, int i)
{
	char	**tmp;
	char	*temp1;
	int		len;
	int		j;
	int		k;

	tmp = ft_strarr_copy(env->env);
	ft_strarr_free(env->env);
	len = 0;
	while (tmp[len])
		len++;
	env->env = ft_calloc(len + 1, sizeof(char *));
	if (!env->env)
		return (1);
	j = -1;
	k = 0;
	while (tmp[++j])
	{
		problem_solver(tmp[j], &temp1);
		if (ft_strncmp(tokens[i], temp1, ft_strlen(temp1)) != 0)
			env->env[k++] = ft_strdup(tmp[j]);
		free(temp1);
	}
	ft_strarr_free(tmp);
	return (0);
}

char	*get_var(char **env, char *envid)
{
	int		i;
	char	*val;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(envid, env[i], strrlen(envid, -1)))
		{
			val = ft_strdup(env[i] + strrlen(env[i], '='));
			return (val);
		}
	}
	return (NULL);
}

int	var_set(char ***tokens)
{
	char	**tmp;
	char	*val;

	val = get_var(env_func()->env, "HOME=");
	if (!val)
		return (0);
	tmp = ft_strarr_copy(*tokens);
	ft_strarr_free(*tokens);
	*tokens = ft_calloc(3, sizeof(char *));
	if (!(*tokens))
		return (2);
	(*tokens)[0] = ft_strdup(tmp[0]);
	(*tokens)[1] = ft_strdup(val);
	ft_strarr_free(tmp);
	free(val);
	return (0);
}
