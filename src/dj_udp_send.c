#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
    struct sockaddr_in s_addr;
    int sock;
    int addr_len;
    int len;
    char buff[128];

    if(argc != 3)
    {
        printf("Usage: ./udpSend ip port \n");
        return -1;
    }

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        perror("socket");
        exit(errno);
    } else
        printf("create socket.\n\r");

    s_addr.sin_family = AF_INET;
    if (argv[2])
        s_addr.sin_port = htons(atoi(argv[2]));
    else
        s_addr.sin_port = htons(1234);

    if (argv[1])
        s_addr.sin_addr.s_addr = inet_addr(argv[1]);
    else 
    {
        printf("server ip not exist.\n");
        exit(0);
    }

    addr_len = sizeof(s_addr);
    strcpy(buff, "hello i'm here");
    len = sendto(sock, buff, strlen(buff), 0,(struct sockaddr *) &s_addr, addr_len);
    if (len < 0) 
    {
        printf("\n\rsend error.\n\r");
        return 3;
    }

    printf("send success.\n\r");
    return 0;
}
