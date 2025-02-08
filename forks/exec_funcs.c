/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 21:58:57 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/07 23:04:14 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int	main(int v, char **str)
{
	pid_t	pid;
	int		bytes_read;
	int		fd[2];
	char	buffer[1000];

	if (v == 2)
	{
		if (pipe(fd) == -1)
			return (0);
		pid = fork();
		if (pid == -1)
			return (0);
		if (pid == 0)
		{
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO); // this function replace the STDOUT_FILENO by fd[1].
			close(fd[1]); // close the original Fd. It was open other replace STDOUT_FILENO by him.
			execlp(str[1], str[1], "-l", NULL);
			perror("execlp error");
			exit(1);
		}
		else
		{
			close(fd[1]);
			wait(NULL);
			bytes_read = read(fd[0], buffer, sizeof(buffer));
			if (bytes_read == -1)
				return (3);
			buffer[bytes_read] = '\0';
			printf("execution of the command passed in terminal : \n %s", buffer);
			close(fd[1]);
		}
	}
	else
		return (-4);
}
