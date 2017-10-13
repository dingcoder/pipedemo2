#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>

#define CHILD_MESS "i want to a cookie\n"
#define PAR_MESS "testing..\n"
#define oops(m,x) {perror(m);exit(x);}

int main() {
    int pipefd[2];
    int len;
    char buf[BUFSIZ];
    int read_len;

    if(pipe(pipefd)==-1)
    {
        oops("Cannot get a pipe",1);
    }

    switch(fork())
    {
        case -1:
            oops("Cannot fork",2);
//            child wirte to pipe every 5 seconds
        case 0:
            len=strlen(CHILD_MESS);
            while(1)
            {
                if(write(pipefd[1],CHILD_MESS,len)!=len)
                {
                    oops("write",3);
                }
                sleep(5);
            }

//            parent reads from pipe and also writes to pipe
        default:
            len=strlen(PAR_MESS);
            while(1)
            {
                if(write(pipefd[1],PAR_MESS,len)!=len)
                {
                    oops("write",4);
                }
                sleep(1);
                read_len=read(pipefd[0],buf,BUFSIZ);
                if(read_len<=0)
                {
                    break;
                }
                write(1,buf,read_len);
            }

    }
    return 0;
}