
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include "../test.h"

#define LISTENQ 5
#define MAXLINE 512

int socket_server_test(){
    printf("+++++socket_server_test start\n");
    int listenfd, connfd;

    socklen_t len;

    struct sockaddr_in servaddr, chiaddr;

    char buff[MAXLINE];

    time_t ticks;

    //create a socket
    listenfd = socket (AF_INET, SOCK_STREAM, 0);

    if(listenfd < 0){
        printf("+++++socket created failed.\n");
        return -1;
    }

    //bind the server ip and port to the socket
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        printf("+++++bind faild.\n");
        return -1;
    }

    //begin to listen on the port
    printf("+++++listening ....\n");
    listen(listenfd, LISTENQ);

    //loop to accept data
    while(1){
        len = sizeof(chiaddr);
        connfd =accept(listenfd, (struct sockaddr *)&chiaddr, &len);

        printf("+++++connect from %s, port %d\n", inet_ntoa(chiaddr.sin_addr.s_addr), ntohs(chiaddr.sin_port));

        ticks = time(NULL);//get the seconds from 1970.01.01

        //put the seconds data to the buff
        sprintf(buff, "%.24s \r\n", ctime(&ticks));
        //write the buff to the client
        write(connfd, buff, strlen(buff));
        //close current client connect
        close(connfd);
    }
}
