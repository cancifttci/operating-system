#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>

#define COMMAND_SIZE 100
char *history[COMMAND_SIZE];
int background_flag = 0;

char** string_parser(char* str){

    char **retval = (char**) malloc(sizeof(char)*10);
    char* first_value = (char*) malloc(sizeof(char)*100);
    retval[0] = first_value;
    int count = 0;
    char *token = strtok(str," ");

    while (token != NULL)
    {      
        char* temp = (char*) malloc(sizeof(char)*100);
        for (int i = 0; i < strlen(token) + 1; i++)
        {   
            temp[i] = token[i];
            temp[i+1] = "\n";
        }
        if (strcmp(token,"&") == 0)
        {   
            //printf(" now setted 1");
            background_flag = 1;
        }
        
        retval[count] = temp;
        count++;
        token = strtok(NULL," ");
        if (count == )
        {
            free(first_value);
        }
    }
    return retval;
}

int add_to_history(char* command){

    for (int i = 10; i >0; i--)
    {
        history[i] = history[i-1];
    } 
    history[0] = command;
    return 0;
}

void show_history(){

    for (int i = 0; i < 10; i++)
    {
        printf("[%d] %s \n",i+1,history[9-i]);
    }
}

void change_directory(char* pathname){

    struct stat sb; 
    char buffer[PATH_MAX];
    getcwd(buffer,sizeof(buffer));
    char *path = pathname;

    strcat(buffer,path);

    char* env = getenv("HOME");
    if (pathname == NULL)
    {
        chdir(env);
        get_current_dir();
        setenv("PWD",env,1);
    }else if(stat(pathname, &sb) == 0 && S_ISDIR(sb.st_mode)){
        chdir(pathname);
    }else if(stat(buffer, &sb) == 0 && S_ISDIR(sb.st_mode)){
        chdir(buffer);
    }else{
        printf(strerror(errno));
        printf("\n");
    }


}

int get_current_dir(){

    char buffer[PATH_MAX];

    if (getcwd(buffer,sizeof(buffer)) != NULL)
    {
        printf("%s\n",buffer);
    }else{

        printf("Error happened when current directory displayed 'getcwd' .");
        return -1;
    }
    return 0;
}

void bye(){

    exit(0);
}

int main(int argc, char const *argv[])
{
    char command[COMMAND_SIZE];

    

    while (1)
    {   
        background_flag = 0;
        printf("myshell> ");
        fgets(command,COMMAND_SIZE,stdin);
        command[strcspn(command,"\n")]=0;
        add_to_history(command);
        
        char** new_temp = string_parser(command);

        if (strcmp(new_temp[0],"cd") == 0)
        {   
            change_directory(new_temp[1]);
        }
        else if (strcmp(new_temp[0],"dir") == 0)
        {
            get_current_dir();
        }
        else if (strcmp(new_temp[0],"history")== 0)
        {
            show_history();
        }
        else if (strcmp(new_temp[0],"bye") == 0)
        {
            bye();
        }
        else
        {
            pid_t pid = NULL;
            int status = 0;
            pid = fork();
            if (pid == 0)
            {
                execvp(new_temp[0],new_temp);
                exit(0);
            }
            else if (pid == -1)
            {
                exit(EXIT_FAILURE);
            }
            else{
                if (!background_flag)
                {   
                    background_flag = 0;
                    waitpid(pid,&status,0);
                }
            }
        }
        free(new_temp);
        
    }

    return 0;
}
