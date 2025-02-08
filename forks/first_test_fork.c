/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_test_fork.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:48:05 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/05 22:22:39 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

int	main(int v, char **str)
{
	int	id;

	(void)v;
	(void)str;
	id = fork();
	if (id != 0)
	{
		fork();
		//printf("hello, my pid is : %i\n", getpid());
		//printf("%s\n", str[1]);
	}
	printf("hi, i'm here\n");
	return (0);
}
