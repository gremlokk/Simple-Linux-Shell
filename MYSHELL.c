/* 
	main.c  
	Authors: Fredrick Ryans
	Instructor: Dr. Bo Sun
	Course: COSC4302            
	Last Modified: 4/21/2017

	Description: Creates a simple shell that allows the use to enter linux commands using execv()
	
	
*/
					

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define MAX_COMMAND_LENGTH 100
#define MAX_NUMBER_OF_PARAMS 10

void prompt()
{
	char cwd[1024];
	getcwd(cwd, 100);
	printf("%s >> ", cwd);
}

// Split cmd into array of parameters
void parseCmd(char* cmd, char** params)
{   
    int i = 0;
    for(i; i < MAX_NUMBER_OF_PARAMS; i++) 
    {
        params[i] = strsep(&cmd, " ");
        if(params[i] == NULL) break;
    }
}

void configurePath(char** params)
{
	char linuxPath[100];
        char linuxCmd[100];
        char *newCmd;
        int length_of_path,length_of_cmd,total_str_length;
        int i;
	char *pathsVars[] = {"/usr/bin/", "/bin/"};//list of commands path to check

	for (i = 0; i < 2; i++)
        {
		strcpy(linuxPath, pathsVars[i]);
        	strcpy(linuxCmd,params[0]);

        	length_of_path = strlen(linuxPath) + 1;
        	length_of_cmd = strlen(linuxCmd) + 1;
        	total_str_length = length_of_path + length_of_cmd;

       		newCmd = (char *)malloc(100);
        	newCmd = strcat(linuxPath,linuxCmd);


        	if (execv(newCmd,params) == -1){execv(newCmd, params);}//line to execute command
	}
}


int executeCmd(char** params)
{
    // Fork process
    pid_t pid = fork();

    // Error
    if (pid == -1) {
        char* error = strerror(errno);
        printf("fork: %s\n", error);
        return 1;
    }

    // Child process
    if (pid == 0) {
        // Execute command
        //execvp(params[0], params);  
 	
	configurePath(params);
	
	// Error occurred
        char* error = strerror(errno);
       
	printf("error: %s: %s\n", params[0], error);
        return 0;
    }

    // Parent process
    // Wait for child process to finish
    int childStatus;
    waitpid(pid, &childStatus, 0);
    return 1;
   
}

int main()
{
    char cmd[100];
    char* params[10];
    int cmdCount = 0;
	char *pathv[10];

    int i;
	/* Allocate memory for char* arrays */
	for(i = 0; i < 10; i++)
	{
		pathv[i] = (char *)malloc(10);
	}

	printf("\t~~~ Welcome to Simple Shell! ~~~\n");
	while(1)
	{
       // Print command prompt
        char* username = getenv("USER");
        
	prompt();
	
        // Read command from standard input, exit on Ctrl+D
        if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;

        // Remove trailing newline character, if any
        if(cmd[strlen(cmd)-1] == '\n') 
		{
            cmd[strlen(cmd)-1] = '\0';
        }

        // Split cmd into array of parameters
        parseCmd(cmd, params);
	   
        // Exit?
        if(strcmp(params[0], "exit") == 0 || executeCmd(params) == 0) break;

	}  
}



