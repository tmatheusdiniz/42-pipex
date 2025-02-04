/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_test_fork.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:17:49 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/04 21:18:59 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int	main(int v, char **str)
{
	(void)v;
	(void)str;
	int id = fork();
	int	x;
	if (id == 0)
	{
		x = 1;
		printf("%i", x);
	}
	else
	{
		x = 6;
		printf("%i", x);
	}
	if (id != 0)
		wait(NULL);
	printf(" ");
	for (int i = x + 1; i < x + 4; i++)
	{
		printf("%i ", i);
		fflush(stdout);
	}
}
