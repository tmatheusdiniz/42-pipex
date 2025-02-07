/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 22:55:42 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/06 23:12:55 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int v, char **str)
{
    int p0[2];
    int p1[2];

    // Create the pipes
    if (pipe(p0) == -1)
        return (-1);
    if (pipe(p1) == -1)
        return (1);

    int pid = fork();
	if (pid == 0)  // Child process
	{
    	close(p0[1]); // Fecha a extremidade de escrita de p0 (não usada)
    	close(p1[0]); // Fecha a extremidade de leitura de p1 (não usada)

    	int x = 42;
    	if (write(p1[1], &x, sizeof(x)) == -1) { // Erro aqui, você está escrevendo no pipe fechado!
        	return 0;
		}
    	int y;
    	if (read(p0[0], &y, sizeof(y)) == -1) { // Erro aqui, você está lendo de um pipe fechado!
			return 0;
    	}

    	printf("received from parent in the child segment : %i\n", y);

    	close(p0[0]);
    	close(p1[1]);
	}
	else  // Parent process
	{
    	close(p0[0]); // Fecha a extremidade de leitura de p0 (não usada)
    	close(p1[1]); // Fecha a extremidade de escrita de p1 (não usada)

    	int y = 10;
    	if (write(p0[1], &y, sizeof(y)) == -1) { // Erro aqui, você está escrevendo no pipe fechado!
        	return 0;
    	}

    	int x;
    	if (read(p1[0], &x, sizeof(x)) == -1) { // Erro aqui, você está lendo de um pipe fechado!
        	return 0;
    	}

    	printf("received from the child in the parent segment : %i\n", x);
    	printf("received and multiplicate by four in the parent seg. : %i\n", x*4);

    	close(p0[1]);
    	close(p1[0]);
	} 

    return 0;
}

