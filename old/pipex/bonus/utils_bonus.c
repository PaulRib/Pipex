/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:11:15 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/15 15:23:48 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
