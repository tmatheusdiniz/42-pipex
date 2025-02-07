/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:49:43 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/06 18:33:05 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

int	main(int v, char **str)
{
	if (mkfifo("fifo01", 0777) == -1)
	{
		if (errno != EEXIST)
			printf("could not create fifo\n");
		return (0);
	}
	int fd = open("fifo01", O_WRONLY);
	int x = 42;
	if (write(fd, &x, sizeof(x)) == -1)
		return (0);
	else
		printf("sucess written");
	close(fd);
}
