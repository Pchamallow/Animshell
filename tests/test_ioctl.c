/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ioctl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 15:50:55 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/01 15:51:27 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int main(void) {
struct termios oldt, newt;
if (tcgetattr(STDIN_FILENO, &oldt) == -1) {
    perror("minishell: tcgetattr");
    return 1;
}

newt = oldt;

// Désactiver le mode canonique et l'écho
newt.c_lflag &= ~(ICANON | ECHO);

if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
    perror("minishell: tcsetattr");
    return 1;
}

printf("Tapez un caractère : ");
char c;
read(STDIN_FILENO, &c, 1);  // lit directement 1 caractère
printf("\\nVous avez tapé : %c\\n", c);

// Revenir au mode normal
tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
return 0;
}
