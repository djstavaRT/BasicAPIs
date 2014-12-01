#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BUFFSIZE 128

int main(int argc, char **argv)
{
    struct sockaddr_in s_addr;
    struct sockaddr_in c_addr;
    int sock;
    socklen_t addr_len;
    int len,wrLen;
    char buff[BUFFSIZE] = {0};
    FILE *fp = NULL;

    if(argc != 2)
    {
        printf("Usage: ./udpReceive port \n");
        return -1;
    }    

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        perror("socket");
        exit(errno);
    } 
    else
        printf("create socket.\n");

    memset(&s_addr, 0, sizeof(struct sockaddr_in));

    s_addr.sin_family = AF_INET;
    if (argv[1])
        s_addr.sin_port = htons(atoi(argv[1]));
    else
        s_addr.sin_port = htons(1234);

    s_addr.sin_addr.s_addr = INADDR_ANY;

    if ((bind(sock, (struct sockaddr *) &s_addr, sizeof(s_addr))) == -1) 
    {
        perror("bind");
        exit(errno);
    } 
    else
        printf("bind address to socket.\n");

    fp = fopen("udp.ts","a+");   
    if(!fp)
    {
        printf("Open udp.ts failed.\n");
    }

    addr_len = sizeof(c_addr);
    while (1) 
    {
        len = recvfrom(sock, buff, sizeof(buff) - 1, 0,(struct sockaddr *) &c_addr, &addr_len);
        if (len < 0) 
        {
            perror("recvfrom");
            exit(errno);
        }

        wrLen = fwrite(buff,1,BUFFSIZE,fp);
        if(wrLen != BUFFSIZE)
            printf("Write length is wrong.\n");
        fflush(fp);

        printf("Received from %s:%d的消息:%s,len = %d\n",inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port), buff,wrLen);
    }
    fclose(fp);
    return 0;
}
