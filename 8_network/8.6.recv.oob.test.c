#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "../test.h"

#define LISTENQ 5
#define PORT 6666

int listenfd, connfd;

const int BUF_SIZE = 100;

void sig_urg(int signo);

int recv_oob_test(){

    int n;
    char buff[BUF_SIZE];
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr;

    struct sigaction action;
    action.sa_handler = sig_urg;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0){
        printf("socket created failed.\n");
        return -1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        printf("bind failed.\n");
        return -1;
    }

    printf("listening ...\n");
    listen(listenfd, LISTENQ);
    len = sizeof(cliaddr);

    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);

    if(sigaction(SIGURG, &action, NULL) == -1){
        printf("could not register signal handler.\n");
        return -2;
    }

    fcntl(connfd, F_SETOWN, getpid());
    while(1){
        if((n = read(connfd, buff, sizeof(buff))) == 0){
            printf("received EOF\n");
            return 0;
        }
        buff[n] = 0;
        printf("read %d bytes:%s\n", n, buff);
    }
}

void sig_urg(int signo){
    int n;
    char buff[100];
    printf("SIGURG received\n");
    n = recv(connfd, buff, sizeof(buff), MSG_OOB);
    buff[n] = 0;
    printf("read %d OOB byte:%s\n", n , buff);
}
