#include <stdio.h>
#include<string.h>
#include<unistd.h>

//definition of the message
const char* WELCOME_MSG="Bienvenue dans le Shell ENSEA. \nPour quitter taper $
const char *ENSEASH="enseash % ";

//writing of the message
int main(){
        write(STDOUT_FILENO, WELCOME_MSG, strlen(WELCOME_MSG));//the massge i$
        write(STDOUT_FILENO, ENSEASH, strlen(ENSEASH));

return 0;
}
