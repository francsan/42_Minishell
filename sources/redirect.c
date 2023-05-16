#include "../headers/minishell.h"

char	*skip_quotes(t_data **d, t_ints *n)
{
	char	*redir;

	n->j = 0;
	while ((*d)->tokens[n->i].token[n->j] != '"' && (*d)->tokens[n->i].token[n->j] != '\'')
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

void	check_redir(t_data **d, int cmd_num)
{
	t_ints	n;
	char	*redir;

	(void) cmd_num;
	n.i = 0;
	while ((*d)->tokens[n.i].token)
	{
		if ((*d)->tokens[n.i].f_redir_input == 1 || (*d)->tokens[n.i].f_redir_output == 1)
		{
			n.j = 0;
			if ((*d)->tokens[n.i].f_singlequotes == 1 || (*d)->tokens[n.i].f_doublequotes == 1)
			{
				redir = skip_quotes(d, &n);
				if (redir[n.j] == '<')
				{
					if (redir[n.j + 1] == '\0')
						(*d)->infile = open((*d)->tokens[n.i + 1].token, O_RDONLY);
					else if (redir[n.j + 1] == '<')
					{
						if (redir[n.j + 2] == '\0')
							(*d)->infile = open((*d)->tokens[n.i + 1].token, O_RDONLY);
						else if (redir[n.j + 2])
							(*d)->infile = open(&redir[n.j + 2], O_RDONLY);
					}
					if (redir[n.j + 1] != '<' && redir[n.j + 1])
						(*d)->infile = open(&redir[n.j + 1], O_RDONLY);
					dup2((*d)->infile, STDIN_FILENO);
					close((*d)->infile);
				}
				else if (redir[n.j] == '>')
				{
					if (redir[n.j + 1] == '\0')
						(*d)->outfile = open((*d)->tokens[n.i + 1].token, O_CREAT | O_TRUNC | O_RDWR, 0666);
					else if (redir[n.j + 1] == '>')
					{
						if (redir[n.j + 2] == '\0')
							(*d)->outfile = open((*d)->tokens[n.i + 1].token, O_CREAT | O_TRUNC | O_RDWR, 0666);
						else if (redir[n.j + 2])
							(*d)->outfile = open(&redir[n.j + 2], O_CREAT | O_TRUNC | O_RDWR, 0666);
					}
					if (redir[n.j + 1] != '>' && redir[n.j + 1])
						(*d)->outfile = open(&redir[n.j + 1], O_CREAT | O_TRUNC | O_RDWR, 0666);
					dup2((*d)->outfile, STDOUT_FILENO);
					close((*d)->outfile);
				}
			}
			else
			{
				if ((*d)->tokens[n.i].token[n.j] == '<')
				{
					if ((*d)->tokens[n.i].token[n.j + 1] == '<')
					{
						if ((*d)->tokens[n.i].token[n.j + 2] == '\0')
							(*d)->infile = open((*d)->tokens[n.i + 1].token, O_RDONLY);
						else if ((*d)->tokens[n.i].token[n.j + 2])
							(*d)->infile = open(&(*d)->tokens[n.i].token[n.j + 2], O_RDONLY);
					}
					else if ((*d)->tokens[n.i].token[n.j + 1] == '\0')
						(*d)->infile = open((*d)->tokens[n.i + 1].token, O_RDONLY);
					else if ((*d)->tokens[n.i].token[n.j + 1] != '<' && (*d)->tokens[n.i].token[n.j + 1])
						(*d)->infile = open(&(*d)->tokens[n.i].token[n.j + 1], O_RDONLY);
					dup2((*d)->infile, STDIN_FILENO);
					close((*d)->infile);
				}
				else if ((*d)->tokens[n.i].token[n.j] == '>')
				{
					if ((*d)->tokens[n.i].token[n.j + 1] == '>')
					{
						if ((*d)->tokens[n.i].token[n.j + 2] == '\0')
							(*d)->outfile = open((*d)->tokens[n.i + 1].token, O_CREAT | O_TRUNC | O_RDWR, 0666);
						else if ((*d)->tokens[n.i].token[n.j + 2])
							(*d)->outfile = open(&(*d)->tokens[n.i].token[n.j + 2], O_CREAT | O_TRUNC | O_RDWR, 0666);
					}
					else if ((*d)->tokens[n.i].token[n.j + 1] == '\0')
						(*d)->outfile = open((*d)->tokens[n.i + 1].token, O_CREAT | O_TRUNC | O_RDWR, 0666);
					else if ((*d)->tokens[n.i].token[n.j + 1] != '>' && (*d)->tokens[n.i].token[n.j + 1])
						(*d)->outfile = open(&(*d)->tokens[n.i].token[n.j + 1], O_CREAT | O_TRUNC | O_RDWR, 0666);
					dup2((*d)->outfile, STDOUT_FILENO);
					close((*d)->outfile);
				}
			}
		}
		n.i++;
	}
}