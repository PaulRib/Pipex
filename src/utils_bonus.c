/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:11:15 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/15 16:00:13 by pribolzi         ###   ########.fr       */
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

static int	here_doc_hub(char *limiter)
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

void	child_process(char *cmd, char **envp, int fd, int *p_fd)
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

void	parent_process(char *cmd, char **envp, int fd, int *p_fd)
{
	if (dup2(fd, STDOUT_FILENO) == -1)
		exit_handling(0);
	if (dup2(p_fd[0], STDIN_FILENO) == -1)
		exit_handling(0);
	close(p_fd[0]);
	close(fd);
	execute(cmd, envp);
}

void	init_pipex(int argc, char **argv, t_pipex *data)
{
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		data->i = 3;
		if (argc < 6)
			exit_handling(4);
		data->fd_out = open_file(argc, argv, 2);
		data->prev_fd = here_doc_hub(argv[2]);
	}
	else
	{
		data->i = 2;
		data->fd_in = open_file(argc, argv, 0);
		data->fd_out = open_file(argc, argv, 1);
		data->prev_fd = data->fd_in;
	}
}
