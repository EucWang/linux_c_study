
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "../test.h"

#define MAXBUFFSIZE 256
#define PORT 6666
#define HOST_ADDR "127.0.0.1"

int socket_client_test(){
    printf("---socket_client_test start\n");
    int sockfd, n;
    char recvbuff[MAXBUFFSIZE];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("---socket created failed.\n");
        return -1;
    }


    memset(&servaddr, 0 ,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
//    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(inet_aton(HOST_ADDR, &servaddr.sin_addr) < 0){
        fprintf(stderr, "inet_aton error.\n");
        exit(1);
    }
    servaddr.sin_port = htons(8080);

    printf("connecting...\n");
    if(connect (sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        printf("connect server failed.\n");
        return -1;
    }

    while((n = read(sockfd, recvbuff, MAXBUFFSIZE)) > 0){
        recvbuff[n] = 0;
        fputs(recvbuff, stdout);
    }

    if(n < 0){
        printf("Read failed.\n");
        return -2;
    }
    return 0;
}
