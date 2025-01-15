/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:28:55 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/15 15:23:53 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	here_doc_process(char *limiter, int len, int *p_fd)
{
	char	*line;

	while (1)
	{
		line = get_next_line(0);
		if (!line)
			exit_handling(0);
		if (!ft_strncmp(line, limiter, len) && line[len] == '\n')
		{
			free(line);
			close(p_fd[0]);
			close(p_fd[1]);
			exit(0);
		}
		ft_putstr_fd(line, p_fd[1]);
		free(line);
	}
}

int	here_doc_hub(char *limiter)
{
	pid_t	pid;
	int		len;
	int		p_fd[2];

	if (pipe(p_fd) == -1)
		exit_handling(0);
	len = ft_strlen(limiter);
	pid = fork();
	if (pid == -1)
		exit_handling(0);
	if (pid == 0)
		here_doc_process(limiter, len, p_fd);
	close(p_fd[1]);
	wait(NULL);
	return (p_fd[0]);
}

static void	child_process(char *cmd, char **envp, int fd, int *p_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit_handling(0);
	if (pid == 0)
	{
		close(p_fd[0]);
		if (dup2(fd, STDIN_FILENO) == -1)
			exit_handling(0);
		if (dup2(p_fd[1], STDOUT_FILENO) == -1)
			exit_handling(0);
		close(p_fd[1]);
		close(fd);
		execute(cmd, envp);
	}
	wait(NULL);
}

static void	parent_process(char *cmd, char **envp, int fd, int *p_fd)
{
	if (dup2(fd, STDOUT_FILENO) == -1)
		exit_handling(0);
	if (dup2(p_fd[0], STDIN_FILENO) == -1)
		exit_handling(0);
	close(p_fd[0]);
	close(fd);
	execute(cmd, envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	data;
	int		p_fd[2];

	if (argc < 5)
		exit_handling(1);
	init_pipex(argc, argv, &data);
	while (data.i < argc - 2)
	{
		if (pipe(p_fd) == -1)
			exit_handling(0);
		child_process(argv[data.i], envp, data.prev_fd, p_fd);
		close(data.prev_fd);
		close(p_fd[1]);
		data.prev_fd = p_fd[0];
		data.i++;
	}
	parent_process(argv[data.i], envp, data.fd_out, p_fd);
	return (0);
}
