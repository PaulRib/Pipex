/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:28:55 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/15 15:59:11 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
