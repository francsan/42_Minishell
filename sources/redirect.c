/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 00:02:10 by francisco         #+#    #+#             */
/*   Updated: 2023/08/08 20:15:53 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*skip_quotes(t_data **d, t_ints *n)
{
	char	*redir;

	n->j = 0;
	while ((*d)->tokens[n->i].token[n->j] != '"' \
		&& (*d)->tokens[n->i].token[n->j] != '\'')
		n->j++;
	if ((*d)->tokens[n->i].token[n->j] == '"')
	{
		n->j++;
		while ((*d)->tokens[n->i].token[n->j] != '"')
			n->j++;
		n->j++;
	}
	else if ((*d)->tokens[n->i].token[n->j] == '\'')
	{
		n->j++;
		while ((*d)->tokens[n->i].token[n->j] != '\'')
			n->j++;
		n->j++;
	}
	redir = ft_strdup(&(*d)->tokens[n->i].token[n->j]);
	return (redir);
}

void	get_eof(char **delimiter, int size)
{
	int		i;
	int		j;
	char	*tmp;

	tmp = ft_calloc(size + 1, sizeof(char));
	i = 0;
	j = 0;
	while (delimiter[i])
		i++;
	while ((*delimiter)[i - 1] != '/')
		i--;
	while (delimiter[i])
		tmp[j++] = (*delimiter)[i++];
	free(*delimiter);
	*delimiter = ft_strdup(tmp);
	free(tmp);
}

void	heredoc_signal(int signal)
{
	if (signal == SIGINT)
	{
		g_exitvalue = 130;
		rl_on_new_line();
		rl_replace_line("", 0);
		exit(1);
	}
	else
		exit(1);
}

void	handle_heredoc(t_data **d, char *delimiter)
{
	t_ints	n;
	char	*buffer;
	int		temp_fd;

	signal(SIGINT, heredoc_signal);
	signal(SIGQUIT, heredoc_signal);
	temp_fd = open("../.tmp", O_CREAT | O_TRUNC | O_WRONLY, 0666);
	write(STDIN_FILENO, "> ", 2);
	buffer = get_next_line(STDIN_FILENO);
	n.a = 0;
	while (delimiter[n.a])
	{
		if (delimiter[n.a] == '/')
			get_eof(&delimiter, ft_strlen(buffer));
		n.a++;
	}
	while (1)
	{
		if (!buffer)
			break ;
		signal(SIGINT, heredoc_signal);
		signal(SIGQUIT, heredoc_signal);
		if (ft_strlen(buffer) - 1 >= ft_strlen(delimiter) \
			&& ft_strncmp(buffer, delimiter, ft_strlen(buffer) - 1) == 0)
			break ;
		else if (ft_strlen(buffer) - 1 < ft_strlen(delimiter) \
			&& ft_strncmp(buffer, delimiter, ft_strlen(delimiter)) == 0)
			break ;
		if (check_for_dollar(&n, buffer))
			expand_dollar_var(&n, &buffer);
		write(temp_fd, buffer, ft_strlen(buffer) + 1);
		free(buffer);
		write(STDIN_FILENO, "> ", 2);
		buffer = get_next_line(STDIN_FILENO);
	}
	free(buffer);
	(*d)->infile = open("../.tmp", O_RDONLY);
}

void	handle_redirections(t_data **d, t_ints *n, char *redir)
{
	if (redir[0] == '<')
	{
		redirect_input(d, n, redir);
	}
	else if (redir[0] == '>')
	{
		redirect_output(d, n, redir);
	}
}

void	check_redir(t_data **d, int cmd_num)
{
	t_ints	n;
	char	*redir;

	n.i = cmd_num;
	while ((*d)->tokens[n.i].token && (*d)->tokens[n.i].f_pipe == 0)
	{
		if ((*d)->tokens[n.i].f_r_in == 1 || (*d)->tokens[n.i].f_r_out == 1)
		{
			if ((*d)->tokens[n.i].f_singlequotes == 1 \
				|| (*d)->tokens[n.i].f_doublequotes == 1)
			{
				redir = skip_quotes(d, &n);
				handle_redirections(d, &n, redir);
				free(redir);
			}
			else
				handle_redirections(d, &n, (*d)->tokens[n.i].token);
		}
		if ((*d)->infile < 0)
		{
			g_exitvalue = errno;
			exit(g_exitvalue);
		}
		n.i++;
	}
}
