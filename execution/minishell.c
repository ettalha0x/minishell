/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nettalha <nettalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 17:36:15 by nettalha          #+#    #+#             */
/*   Updated: 2023/06/23 15:48:05 by nettalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

glb	g_glb;

int	execute(t_cmd	*cmd, t_env **my_envp)
{
	pid_t	pid;
	int		error;
	char	**envp;
	char	*valid_path;

	error = 0;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		valid_path = get_valid_path(cmd, *my_envp, &error);
		if (valid_path && !error)
		{
			envp = struct_to_env(my_envp);
			if (execve(valid_path, cmd->cmd, envp) == -1)
			{
				ft_error(cmd->cmd[0], strerror(errno), errno);
				exit(errno);
			}
		}
		else if (!valid_path && error == 2)
		{
			ft_error(cmd->cmd[0], "command not found", errno);
			exit(127);
		}
	}
	return (pid);
}

int	main(int ac, char **av, char **envp)
{
	t_cmd	*cmd;
	char	*line;
	t_env	*my_envp;
	pid_t	pid;

	(void)ac;
	(void)av;
	g_glb.o_stdin = dup(STDIN_FILENO);
	g_glb.o_stdout = dup(STDOUT_FILENO);
	my_envp = env_to_struct(envp);
	using_history();
	while (1)
	{
		ft_signals();
		line = readline("minishell$ ");
		if (!line)
		{
			free(line);
			exit (g_glb.exit_status);
		}
		if (*line)
		{
			add_history(line);
			cmd = parsing1(line, my_envp);
			if (!cmd || (!cmd->cmd[0] && !cmd->red))
			{
				free(line);
				continue ;
			}
		}
		else
		{
			free(line);
			continue ;
		}
		if (cmd->Rpipe == 0)
		{
			if (!check_red(cmd))
			{
				free(line);
				continue ;
			}
			if (cmd->cmd[0])
			{
				if (builtins(cmd, my_envp))
				{
					dup2(g_glb.o_stdin, STDIN_FILENO);
					dup2(g_glb.o_stdout, STDOUT_FILENO);
					free(line);
					free_ll(cmd);
					continue ;
				}
				pid = execute(cmd, &my_envp);
				waitpid(pid, &g_glb.status, 0);
				g_glb.exit_status = WEXITSTATUS(g_glb.status);
			}
		}
		else
		{
			dup2(g_glb.o_stdin, STDIN_FILENO);
			dup2(g_glb.o_stdout, STDOUT_FILENO);
			ft_pipe(cmd, &my_envp);
		}
		dup2(g_glb.o_stdin, STDIN_FILENO);
		dup2(g_glb.o_stdout, STDOUT_FILENO);
		free_ll(cmd);
		free(line);
	}
	return (0);
}
