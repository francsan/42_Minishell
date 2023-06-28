/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:40:22 by francsan          #+#    #+#             */
/*   Updated: 2023/06/28 17:13:03 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_signal	*sig_func(void)
{
	static t_signal	sig;

	return (&sig);
}

void	sig_handler(int sig)
{
	if (sig == SIGQUIT)
		return ;
	if (sig == SIGINT)
	{
		g_exitvalue = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	return ;
}

void	if_ctrl_d(t_data **d, char *buffer, char *line)
{
	unsigned char	status;

	if (line)
		return ;
	rl_clear_history();
	write(1, "exit\n", 5);
	ft_strarr_free((*d)->paths);
	ft_strarr_free((*d)->env);
	free(buffer);
	free(line);
	status = (unsigned char)(sig_func())->exit_status;
	exit(status);
}
