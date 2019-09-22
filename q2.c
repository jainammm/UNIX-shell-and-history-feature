#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main( )
{

pid_t pid;
int fd1[2];
int fd2[2];


if (pipe(fd1) == -1) {
fprintf(stderr,"Pipe failed");
return 1;
}

if(pipe(fd2) == -1){
        fprintf(stderr,"Pipe failed");
return 1;
}

pid = fork();

if (pid < 0) {
fprintf(stderr, "Fork failed");
return 1;
}

if (pid > 0) {  
       
        printf("Inside the parent \n");
        printf("Enter Input String \n");
   char input[BUFFER_SIZE];
        scanf("%s",input);

   char inverted[BUFFER_SIZE];

        close(fd1[READ_END]);
        write(fd1[WRITE_END], input, strlen(input)+1);
        close(fd1[WRITE_END]);
       

        close(fd2[WRITE_END]);
        read(fd2[READ_END],inverted,BUFFER_SIZE);
        printf("Reading inverted string \n");
        close(fd2[READ_END]);
       
        printf("Inverted String - %s\n", inverted);        
}
else {

   char input[BUFFER_SIZE];
   printf("Inside child process \n");

    close(fd1[WRITE_END]);
    read(fd1[READ_END], input, BUFFER_SIZE);
    printf("Reading Input string from parent\n");
    close(fd1[READ_END]);

            int i=0;
            while(input[i]!='\0'){

                if(input[i]>=65&&input[i]<=90){
                input[i]+=32;
                }
                else if(input[i]>=97&&input[i]<=122){
                input[i]-=32;
                }

                i++;
            }


            close(fd2[READ_END]);
    write(fd2[WRITE_END], input, strlen(input)+1);
    printf("Sending inverted string \n");
    close(fd2[WRITE_END]);

    }

    return 0;
}

