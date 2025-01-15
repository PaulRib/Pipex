/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 15:29:38 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/14 17:07:58 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	open_file(int argc, char **argv, int i)
{
	int	fd;

	if (i == 0)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			exit_handling(5);
	}
	else if (i == 1)
	{
		fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd == -1)
			exit_handling(5);
	}
	if (i == 2)
	{
		fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fd == -1)
			exit_handling(5);
	}
	return (fd);
}

void	exit_handling(int err)
{
	if (err == 0)
		ft_putstr_fd("An error was encountered\n", 2);
	else if (err == 1)
		ft_putstr_fd("Usage : ./pipex file1 cmd1 cmd2 file2\n", 2);
	else if (err == 2)
		ft_putstr_fd("Empty command\n", 2);
	else if (err == 3)
	{
		ft_putstr_fd("Error : Command not found\n", 2);
		exit(127);
	}
	else if (err == 4)
		ft_putstr_fd("Usage : ./pipex here_doc LIMITER cmd cmd1 file\n", 2);
	else if (err == 5)
	{
		ft_putstr_fd("A problem with open happened\n", 2);
		return ;
	}
	exit(1);
}

void	free_tab(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	*get_path(char *cmd, char **envp, char **exec_cmd)
{
	int		i;
	char	**path;
	char	*good_path;
	char	*temp;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH", 4) != 0)
		i++;
	path = ft_split(envp[i] + 5, ':');
	i = 0;
	while (path[i])
	{
		temp = ft_strjoin(path[i], "/");
		good_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(good_path, F_OK) == 0)
			return (free_tab(path), good_path);
		i++;
		free(good_path);
	}
	free_tab(exec_cmd);
	free_tab(path);
	exit_handling(3);
	return (NULL);
}

void	execute(char *cmd, char **envp)
{
	char	**exec_cmd;
	char	*path;

	if (!cmd || !cmd[0])
		exit_handling(2);
	exec_cmd = ft_split(cmd, ' ');
	path = get_path(exec_cmd[0], envp, exec_cmd);
	if (execve(path, exec_cmd, envp) == -1)
	{
		free_tab(exec_cmd);
		free(path);
		exit_handling(0);
	}
}
