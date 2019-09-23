#include <stdio.h>
#include <string.h> 
#include<sys/wait.h> 
#include <unistd.h>


#define MAX_LINE 80 /* The maximum length command */

#define BUFFER_SIZE 100

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
        // printf("\n jainam %d", commandStr[0]);

        int i=0, ampersandCheck = 0;
        char* token = strtok(commandStr, " "); 

        while (token != NULL) {  
            if(token[strlen(token)-1] - '\n' == 0){
                token[strlen(token)-1] = '\0';
            }

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

        if (strcmp(args[0], "history") == 0){
            if(count>0){
                printf("Shell command history:\n");
                int k;
                int j = 0;
                int histCount = count;
                
                for (k = 0; k<10;k++)
                {

                    printf("%d.  ", histCount);
                    while (history[k][j] != '\n' && history[k][j] != '\0')
                    {	
                        printf("%c", history[k][j]);
                        j++;
                    }
                    printf("\n");
                    j = 0;
                    histCount--;
                    if (histCount ==  0)
                        break;
                }
		    }
            else
            {
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
            int x = args[0][1]- '0'; 
            int z = args[0][2]- '0'; 
            if (x >= count){
                printf("No such command!\n");
                continue;
            }

            else if (z!=-48) {
                printf("No Such Command in the history. Enter <=!9 (buffer size is 10 along with current command)\n");
                continue;
            }

            prevExecuted = 1;
            strcpy(commandStr, history[x]);
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

            strcpy(history[0],commandStr); //Updating the history array with input buffer
            
            count++;
            if(count>10){
                count=10;
            }
        }
        int status;
        int pid = fork();

        if(pid == 0){
            execvp(args[0], args);
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

