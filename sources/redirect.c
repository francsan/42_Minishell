/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 00:02:10 by francisco         #+#    #+#             */
/*   Updated: 2023/06/28 17:39:30 by francsan         ###   ########.fr       */
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

void	handle_heredoc(t_data **d, char *delimiter)
{
	char	*buffer;
	int		temp_fd;

	temp_fd = open("../.tmp", O_CREAT | O_TRUNC | O_WRONLY, 0666);
	write(STDIN_FILENO, "> ", 2);
	buffer = get_next_line(STDIN_FILENO);
	while (1)
	{
		if (ft_strncmp(buffer, delimiter, ft_strlen(buffer) - 1) == 0 \
			&& ft_strlen(buffer) > 1)
			break ;
		write(temp_fd, buffer, ft_strlen(buffer));
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
		if (redir[1] == '\0')
			(*d)->infile = open((*d)->tokens[n->i + 1].token, O_RDONLY);
		else if (redir[1] == '<')
		{
			if (redir[2] == '\0')
				handle_heredoc(d, (*d)->tokens[n->i + 1].token);
			else if (redir[2])
				handle_heredoc(d, &redir[2]);
		}
		else if (redir[1] != '<' && redir[1])
			(*d)->infile = open(&redir[1], O_RDONLY);
		dup2((*d)->infile, STDIN_FILENO);
		close((*d)->infile);
	}
	else if (redir[0] == '>')
	{
		if (redir[1] == '\0')
			(*d)->outfile = open((*d)->tokens[n->i + 1].token, \
				O_CREAT | O_TRUNC | O_WRONLY, 0666);
		else if (redir[1] == '>')
		{
			if (redir[2] == '\0')
				(*d)->outfile = open((*d)->tokens[n->i + 1].token, \
				O_CREAT | O_APPEND | O_WRONLY, 0666);
			else if (redir[2])
				(*d)->outfile = open(&redir[2], \
				O_CREAT | O_APPEND | O_WRONLY, 0666);
		}
		else if (redir[1] != '>' && redir[1])
			(*d)->outfile = open(&redir[1], \
				O_CREAT | O_TRUNC | O_WRONLY, 0666);
		dup2((*d)->outfile, STDOUT_FILENO);
		close((*d)->outfile);
	}
}

void	check_redir(t_data **d, int cmd_num)
{
	t_ints	n;
	char	*redir;

	n.i = cmd_num;
	while ((*d)->tokens[n.i].token && (*d)->tokens[n.i].f_pipe == 0)
	{
		if ((*d)->tokens[n.i].f_redir_input == 1 \
			|| (*d)->tokens[n.i].f_redir_output == 1)
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
