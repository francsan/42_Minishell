#include "../headers/minishell.h"

t_red   *add_red(char *file, int is_two, int output, t_red **beg)
{
	t_red	*new;
	t_red	*tmp;

	tmp = NULL;
	new = ft_calloc(1, sizeof(t_red));
	if (!new)
		return (NULL);
	new->fd = -1;
	new->file = file;
	new->is_two = is_two;
	new->output = output;
	new->next = NULL;
	if (!(*beg))
	{
		*beg = new;
		return (new);
	}
	tmp = *beg;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (new);
}

int pass_red(char *tokens, t_cmd *cmd, int i)
{
	if (!tokens)
		return (1);
	if (i == 1)
		add_red(tokens, 0, 1, &cmd->io);
	if (i == 2)
		add_red(tokens, 0, 0, &cmd->io);
	if (i == 3)
		add_red(tokens, 1, 1, &cmd->io);
	if (i == 4)
		add_red(tokens, 1, 0, &cmd->io);
	return (0);
}

int check_redirection(char **tokens, t_cmd *cmd, int i)
{
	if (tokens[i][0] == '>')
		pass_red(tokens[i + 1], cmd, 1);
	if (tokens[i][0] == '<')
		pass_red(tokens[i + 1], cmd, 2);
	if (tokens[i][1] && tokens[i][0] == '>' && tokens[i][1] == '>')
		pass_red(tokens[i + 1], cmd, 3);
	if (tokens[i][1] && tokens[i][0] == '<' && tokens[i][1] == '<')
		pass_red(tokens[i + 1], cmd, 4);
	return (0);
}