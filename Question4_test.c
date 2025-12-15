#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    pid_t my_pid = getpid();
    printf("Programme victime lancé. Mon PID est : %d\n", my_pid);
    printf("Je vais maintenant boucler indéfiniment...\n");

    // Boucle infinie pour maintenir le programme en vie
    while (1) {
        // Optionnel : un petit affichage pour prouver que le programme tourne
        fflush(stdout); 
        sleep(1); 
    }

    return 0; // Inaccessible, sauf si break
}
