/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atarchou <atarchou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 20:49:22 by atarchou          #+#    #+#             */
/*   Updated: 2022/08/22 11:40:28 by atarchou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

int	ft_find_last_character(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}

int	ft_find_variable_index(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (i);
}

char	*ft_get_expand_env(char *str)
{
	int		i;
	char	*ret;

	i = 0;
	ret = malloc(sizeof(char *) * (ft_find_variable_index(str, '=') + 1));
	while (i < ft_find_variable_index(str, '='))
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

char	*ft_mystrdu_norm(char *str, char *s1, int i, int j)
{
	str = malloc(sizeof(char) * ft_strlen(s1) - 1);
	while (i < ft_strlen(s1) - 1)
	{
		if (s1[i] == '\"')
			i++;
		str[j] = s1[i];
		i++;
		j++;
	}
	str[j] = '\0';
	return (str);
}

char	*ft_mystrdup(char *s1, int flag)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	if (flag)
	{
		str = NULL;
		str = ft_mystrdu_norm(str, s1, i, j);
		return (str);
	}
	else
	{
		str = malloc(sizeof(char) * ft_strlen(s1) + 1);
		while (s1[i])
		{
			str[i] = s1[i];
			i++;
		}
		str[i] = '\0';
	}
	return (str);
}

int	ft_count_elements(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_get_env(t_exec *exec, char **env)
{
	int	i;

	i = 0;
	exec->err_flag = 0;
	exec->envp = malloc(sizeof(char *) * (ft_count_elements(env) + 2));
	while (i < ft_count_elements(env))
	{
		exec->envp[i] = strdup(env[i]);
		i++;
	}
	exec->envp[i] = 0;
	exec->env.exit_value = 0;
}

char	*ft_expand(char *expand, char **envp)
{
	int		i;
	char	*xtemp;

	i = 0;
	if (expand[0] != '$')
		return (ft_mystrdup(expand, 0));
	if (expand != NULL)
	{
		while (envp[i])
		{
			xtemp = ft_get_expand_env(envp[i]);
			if (ft_strcmp(expand + 1, xtemp) == 0)
			{
				free(xtemp);
				return (ft_mystrdup(envp[i]
						+ ft_find_last_character(envp[i], '=') + 1, 0));
			}
				i++;
			free(xtemp);
		}
	}
	return (ft_mystrdup("", 0));
}

int	main(int argc, char **argv, char **envp)
{
	char *line;
	t_lexer *lexer;
	t_cmd *cmd;
	t_cmd *head;
	t_exec	exec;

	(void)argc;
	(void)argv;
	ft_get_env(&exec, envp);
	while (1)
	{
		ft_signals();
		line = readline("BomusShell$> ");
		if (line == NULL)
			exit(0);
		add_history(line);
		lexer = init_lx(line);
		cmd = create_lst_cmd(lexer, &exec);
		head = cmd;
		fix_quotes(&cmd);
		cmd = head;
		printf("final : |");
		while (cmd)
		{
			//printf("%s", cmd->tok->value);
			 printf("TOKEN (%d, %s, %c)\n", cmd->tok->type, cmd->tok->value, cmd->tok->quote);
			cmd = cmd->next;
		}
		printf("|\n");
		free(line);
	}
}