/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 15:29:22 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/14 16:26:10 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	child_process(char **argv, char **envp, int *p_fd)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		exit_handling(0);
	if (dup2(fd, STDIN_FILENO) == -1)
		exit_handling(0);
	if (dup2(p_fd[1], STDOUT_FILENO) == -1)
		exit_handling(0);
	close(p_fd[0]);
	close(p_fd[1]);
	close(fd);
	execute(argv[2], envp);
}

static void	parent_process(char **argv, char **envp, int *p_fd)
{
	int	fd;

	wait(NULL);
	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		exit_handling(0);
	if (dup2(fd, STDOUT_FILENO) == -1)
		exit_handling(0);
	if (dup2(p_fd[0], STDIN_FILENO) == -1)
		exit_handling(0);
	close(p_fd[1]);
	close(p_fd[0]);
	close(fd);
	execute(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid;

	if (argc != 5)
		exit_handling(1);
	if (pipe(fd) == -1)
		exit_handling(0);
	pid = fork();
	if (pid < 0)
		exit_handling(0);
	if (pid == 0)
		child_process(argv, envp, fd);
	else
		parent_process(argv, envp, fd);
	return (0);
}
