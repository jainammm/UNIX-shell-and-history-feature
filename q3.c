#include <stdio.h>
#include <string.h> 
#include<sys/wait.h> 
#include <unistd.h>


#define MAX_LINE 80 /* The maximum length command */
int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */
    while (should_run) {
        printf("\033[1;32m");       // Adding color
        printf("osh> ");
        printf("\033[0m");
        char commandStr[1000];
        scanf("%[^\n]%*c", commandStr);

        int i=0, ampersandCheck = 0;
        char* token = strtok(commandStr, " "); 

        while (token != NULL) {             
            if(strcmp(token, "&") == 0){
                ampersandCheck=1;
            }
            else{
                args[i++] = token;
            }

            token = strtok(NULL, " "); 

        }
        args[i] = NULL;

        if(strcmp(args[0], "exit") == 0){
            should_run = 0;
            continue;
        }

        int pid = fork();

        if(pid == 0){
            execvp(args[0], args);
        }
        else{
            if(ampersandCheck == 0){
                wait(NULL);
            }
        }



        fflush(stdin);

        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) if command included &, parent will invoke wait()
        */
    }
    return 0;
}

