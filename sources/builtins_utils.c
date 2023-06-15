/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:52:28 by francsan          #+#    #+#             */
/*   Updated: 2023/06/15 20:26:43 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	is_num(char *str)
{
	int	i;

	i = 0;
	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] < '0' && str[i] > '9')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i])
		return (0);
	return (1);
}

void export_print(char **env, int out)
{
	int	i;
	int	len;

	i = -1;
	while (env[++i])
	{
		len = strrlen(env[i], '=');
		write(out, "declare -x ", 11);
		write(out, env[i], len);
		if (env[i][len - 1] != '=')
			write(out, "=", 1);
		write(out, "\"", 1);
		if (env[i][len])
			write(out, env[i] + len, strrlen(env[i] + len, -1));
		write(out, "\"\n", 2);
	}
}

int var_pos(char *str, char **env)
{
	int	i;

	i = -1;

	while (env[++i])
		if (!ft_strncmp(str, env[i], strrlen(env[i], '=') - 1))
			return (i);
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

int	handle_no_var(char **tokens, t_env *env, int i)
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

int  is_valid(char *str)
{
	int	i;

	i = -1;
	while (str[++i] && str[i] != '=')
	{
		if (i == 0 && !((str[i] >= 'a' && str[i] <= 'z') || \
		(str[i] >= 'A' && str[i] <= 'Z')))
			return (0);
		if (i && ft_isalnum(str[i]))
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

int	env_set(t_cmd *cmd, t_env *env, int i)
{
	char **tmp;
	int len;
	int	j;
	int k;

	tmp = env->env;
	len = -1;
	while (env->env[++len])
		;
	env->env = ft_calloc(len, sizeof(char *));
	if (!env->env)
		return (1);
	j = -1;
	k = 0;
	while (tmp[++j])
	{
		if (!ft_strncmp(cmd->args[i], tmp[j], ft_strlen(cmd->args[i] -1)))
			free(tmp[j]); 
		else
			env->env[++k] = tmp[j];
	}
	free(tmp);
	return (0);
}

int env_change(t_env *env)
{
	char	*tmp;
	int     i;

	i = -1;
	while(env->env[++i])
		if(!ft_strncmp(env->env[i], "PWD=", 4))
			break;
	tmp = ft_strjoin("OLD", env->env[i]);
	free(env->env[i]);
	env->env[i] = tmp;
	i = -1;
	while(env->env[++i])
		if (!strncmp(env->env[i], "OLDPWD=", 7))
			break ;
	tmp = getcwd(NULL, 0);
	free(env->env[i]);
	env->env[i] = ft_strjoin("PWD=", tmp);
	free(tmp);
	return (0);
}

char *get_var(char **env, char *envid) //static?
{
	int     i;
	char    *val;

	i = -1;
	while(env[++i])
	{
		if (!ft_strncmp(envid, env[i], strrlen(envid, -1)))
		{
			val = ft_strdup(env[i] + strrlen(env[i], '='));
			return (val);
		}
	}
	return (NULL);
}

int var_set(char **tokens)
{
	char **tmp;
	char *val;

	val = get_var(env_func()->env, "HOME=");
	if (!val)
		return (0);/*return(*print_error(cmd, HOME not set))*/
	tmp = tokens;
	tokens = ft_calloc(3, sizeof(char *));
	if (!tokens)
		return (2);
	tokens[0] = tmp[0];
	tokens[1] = val;
	free(tmp);
	return(0);
}