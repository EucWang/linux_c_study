#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include "../test.h"
#define PORT 6666
#define HOST_ADDR "127.0.0.1"

int send_oob_test(){

    int sockfd, n;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("socket created failed.\n");
        return -1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    printf("connecting...\n");

    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        printf("connect error.\n");
        return -1;
    }

    write(sockfd, "123", 3);
    printf("wrote 3 byte of normal data.\n");
    sleep(1);

    send(sockfd, "a", 1,MSG_OOB);
    printf("wrote 1 byte of OOB data.\n");
    sleep(1);

    write(sockfd, "56", 2);
    printf("wrote 2 byte of normal data.\n");
    sleep(1);

    send(sockfd, "b", 1, MSG_OOB);
    printf("wrote 1 byte of OOB data.\n");
    sleep(1);

    write(sockfd, "89",2);
    printf("wrote 2 byte of normal data.\n");
    sleep(1);
    return 0;
}
