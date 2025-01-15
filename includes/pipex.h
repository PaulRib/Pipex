/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:29:49 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/15 16:02:15 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int	i;
	int	fd_in;
	int	fd_out;
	int	prev_fd;
}		t_pipex;

void	free_tab(char **str);
char	*get_path(char *cmd, char **envp, char **exec_cmd);
void	execute(char *cmd, char **envp);
void	exit_handling(int err);
int		open_file(int argc, char **argv, int i);
void	init_pipex(int argc, char **argv, t_pipex *data);
void	child_process(char *cmd, char **envp, int fd, int *p_fd);
void	parent_process(char *cmd, char **envp, int fd, int *p_fd);

#endif