/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:19:19 by francisco         #+#    #+#             */
/*   Updated: 2023/04/26 10:12:08 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// JUST FOR TESTING
void	print_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		printf("%s\n", arr[i]);
}

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

int	main(int argc, char **argv, char **envp)
{
	t_data	*d;
	char	*line;

	(void)argc;
	(void)argv;
	d = ft_calloc(1, sizeof(t_data));
	d->env = ft_strarr_copy(envp);
	get_paths(&d);
	while (1)
	{
		line = readline(PROMPT);
		if (line && ft_strlen(line) > 0)
			add_history(line);
		if (parse_command(d, line))
			break ;
		free(line);
	}
	ft_strarr_free(d->paths);
	ft_strarr_free(d->env);
	free(line);
	free(d);
}
