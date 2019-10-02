#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include<sys/wait.h> 
#include <unistd.h>


#define MAX_LINE 80 /* The maximum length command */

#define BUFFER_SIZE 100

void displayHistory(char history[10][BUFFER_SIZE], int histCount){
    printf("Shell command history:\n");
    int k = 0;
    if(histCount > 10){
        k = histCount - 10;
    }
    int j = 0;
    for (int i=0; i <= 10;i++){
        printf("%d.  ", histCount);
        while (history[i][j] != '\n' && history[i][j] != '\0')
        {	
            printf("%c", history[i][j]);
            j++;
        }
        printf("\n");
        j = 0;
        histCount--;
        if (k >= histCount){
            break;
        }
    }
}

int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */
    int count = 0;
    char history[10][BUFFER_SIZE];
    while (should_run) {
        printf("\033[1;32m");       // Adding color
        printf("osh> ");
        printf("\033[0m");          // Removing color

        char commandStr[BUFFER_SIZE];
        fgets(commandStr, BUFFER_SIZE, stdin); 
        // scanf("%[^\n]%*c", commandStr);

        if(commandStr[strlen(commandStr)-1] - '\n' == 0){
            commandStr[strlen(commandStr)-1] = '\0';
        }

        int i=0, ampersandCheck = 0;
        char* token = strtok(commandStr, " "); 

        if(strcmp(token, "exit") == 0){
            should_run = 0;
            continue;
        }

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

        if (strcmp(args[0], "history") == 0){
            if(count>0){
                displayHistory(history, count);     // Utility function for displaying history
		    }
            else{
                printf("No Commands in the history!\n");
            }

            continue;
        }

        int prevExecuted = 0;
        if(strcmp(args[0], "!!") == 0){
            if(count > 0){
                prevExecuted = 1;
                strcpy(commandStr, history[0]);
                token = strtok(commandStr, " "); 
                i=0;
                ampersandCheck=0;
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
            }
            else{
                printf("No recent command in the history!");
                continue;
            }
        }

        if(args[0][0] - '!' == 0){
            if(args[0][1] - '\0' == 0){
                printf("Please enter correct command.\n");
                continue;
            }
            int x = count - atoi(args[0]+1); 
            if (x >= 10){
                printf("No such command!\n");
                continue;
            }
            if(x < 0){
                printf("No such command!\n");
                continue;
            }

            prevExecuted = 1;
            strcpy(commandStr, history[x]);
            printf("Command executed: %s\n", commandStr);
            token = strtok(commandStr, " "); 
            i=0;
            ampersandCheck=0;
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

        }

        if(prevExecuted == 0){
            for (i = 9;i>0; i--) //Moving the previous history 
                strcpy(history[i], history[i-1]);

            strcpy(history[0], "");
            for(int i=0; args[i] != NULL; i++){
                strcat(history[0], args[i]);
                strcat(history[0], " ");
            }
            
            count++;
            
        }
        int status;
        int pid = fork();

        if(pid == 0){
            if(execvp(args[0], args) < 0){
                printf("Please enter valid command. \n");
                return -1;
            }
        }
        else{
            if(ampersandCheck == 0){
                while(wait(&status) != pid)
                    ;
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

