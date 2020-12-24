#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MAX_ARGS 10
#define MAX_WORD 30
#define MAX_LINE 100

char whitespace[] = " \t\r\n\v";
char args[MAX_ARGS][MAX_WORD];

void runcmd(char*argv[],int argc);
void execPipe(char*argv[],int argc){
    int a=0;
    for(;a<argc;a++){
        if(!strcmp(argv[a],"|")){
            argv[a]=0;
            break;
        }
    }
    int Fd[2];
    pipe(Fd);
    if(fork()==0){
        close(1);
        dup(Fd[1]);
        close(Fd[0]);
        close(Fd[1]);
        runcmd(argv,a);
    }else{
        close(0);
        dup(Fd[0]);
        close(Fd[0]);
        close(Fd[1]);
        runcmd(argv+a+1,argc-a-1);
    }
}

int getcmd(char *buf, int nbuf)
{
    fprintf(2, "@ ");
    memset(buf, 0, nbuf);
    gets(buf, nbuf);
    if (buf[0] == 0) 
        return -1;
    return 0;
}

void setargs(char *cmd, char* argv[],int* argc)
{
    for(int x=0;x<MAX_ARGS;x++){
        argv[x]=&args[x][0];
    }
    int a = 0; 
    int b = 0;
    for (; cmd[b] != '\n' && cmd[b] != '\0'; b++){
        while (strchr(whitespace,cmd[b])){
            b++;
        }
        argv[a++]=cmd+b;
        while (strchr(whitespace,cmd[b])==0){
            b++;
        }
        cmd[b]='\0';
    }
    argv[a]=0;
    *argc=a;
}

void runcmd(char*argv[],int argc)
{
    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i],"|")){
            execPipe(argv,argc);   
        }
    }
    for(int i=1;i<argc;i++){   
        if(!strcmp(argv[i],">")){
            close(1);
            open(argv[i+1],O_CREATE|O_WRONLY);  
            argv[i]=0;
        }
        if(!strcmp(argv[i],"<")){ 
            close(0);
            open(argv[i+1],O_RDONLY);
            argv[i]=0;
        }
    }
    exec(argv[0], argv);
}
 

int main()
{
    char buf[MAX_LINE];
    while (getcmd(buf, sizeof(buf)) >= 0)
    {
 
        if (fork() == 0)
        {
            char* argv[MAX_ARGS];
            int argc=-1;
            setargs(buf, argv, &argc);
            runcmd(argv,argc);
        }
        wait(0);
    }
    exit(0);
}