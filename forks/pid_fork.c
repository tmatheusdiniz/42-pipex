/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:19:13 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/04 23:11:01 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

int	main(int v, char **str)
{
	int	pid;

	pid = fork();
	printf("Current process %i :  parent process : %i ", getpid(), getppid());
	printf("\n");
	if (pid == 0)
		printf("child process id : %i\n", getpid());
	else
	 	printf("parent process id : %i\n", getppid());
}
