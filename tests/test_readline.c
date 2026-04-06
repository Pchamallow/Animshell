/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_readline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 22:21:25 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/01 22:12:51 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ********************************************* */
/* --------------- test 1 ---------------------- */
/* ********************************************* */

/*#include <stdlib.h>*/
/*#include <unistd.h>*/
/*#include <stdio.h>*/
/*#include <string.h>*/

/*#include <readline/readline.h>*/
/*#include <readline/history.h>*/

/*int*/
/*main (void)*/
/*{*/
    /*char *input;*/

    /*for (;;) {*/
        /*input = readline((char *)NULL);*/
        /*if (input == 0)*/
            /*break;*/
        /*printf ("%s\n", input);*/
        /*if (strcmp (input, "exit") == 0)*/
            /*break;*/
        /*free (input);*/
    /*}*/
    /*exit (0);*/
/*}*/

/* ********************************************* */
/* --------------- test 2 ---------------------- */
/* ********************************************* */

/*#include <stdio.h>*/
/*#include <stdlib.h>*/
/*#include <readline/readline.h>   // For readline()*/
/*#include <readline/history.h>    // For add_history()*/

/*int main() {*/
    /*char *input;*/

    /*// Prompt the user and read input with readline()*/
    /*input = readline("Enter your name: ");*/

    /*// Check if input is NULL (user pressed Ctrl+D to exit)*/
    /*if (input == NULL) {*/
        /*printf("\nExiting...\n");*/
        /*return 0;*/
    /*}*/

    /*// Add input to history (so user can recall it with Up arrow)*/
    /*add_history(input);*/

    /*// Print the input*/
    /*printf("Hello, %s!\n", input);*/

    /*// Free the memory allocated by readline()*/
    /*free(input);*/

    /*return 0;*/
/*}*/

/* ********************************************* */
/* --------------- test 3 ---------------------- */
/* ********************************************* */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>   // For readline()
#include <readline/history.h>    // For add_history()

int main() {
    char *input;

    while (1)
    {
        // Prompt the user and read input with readline()
        input = readline("Minishell>");

        // Check if input is NULL (user pressed Ctrl+D to exit)
        if (input == NULL) {
            printf("\nExiting...\n");
            return 0;
        }

        // Add input to history (so user can recall it with Up arrow)
        add_history(input);

        // Print the input
        printf("%s\n", input);

        // Free the memory allocated by readline()
        free(input);
    }

    return 0;
}
