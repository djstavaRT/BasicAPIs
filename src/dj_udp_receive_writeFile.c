#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFSIZE 128

char buff[BUFFSIZE] = {0};
sem_t sem_receive, sem_writeFile;

void *dj_udp_receive()
{
    struct sockaddr_in s_addr;
    struct sockaddr_in c_addr;
    int sock;
    socklen_t addr_len;
    int len;

	sem_wait(sem_receive);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        perror("socket");
        exit(errno);
    } 
    else
        printf("create socket.\n");

    memset(&s_addr, 0, sizeof(struct sockaddr_in));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(1234);
    s_addr.sin_addr.s_addr = INADDR_ANY;

    if ((bind(sock, (struct sockaddr *) &s_addr, sizeof(s_addr))) == -1) 
    {
        perror("bind");
        exit(errno);
    } 
    else
        printf("bind address to socket.\n");    

    addr_len = sizeof(c_addr);
    while (1) 
    {
        len = recvfrom(sock, buff, sizeof(buff) - 1, 0,(struct sockaddr *) &c_addr, &addr_len);
        if (len < 0) 
        {
            perror("recvfrom");
            exit(errno);
        }

        printf("Received from %s:%d的消息:%s,len = %d\n",inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port), buff,wrLen);
    }
	
}

void *dj_udp_writeFile(FILE *file)
{
    int wrLen = 0;
	
	wrLen = fwrite(buff,1,BUFFSIZE,file);
    if(wrLen != BUFFSIZE)
        printf("Write length is wrong.\n");
    fflush(file);
}

int main()
{
    int i=0, ret=0;
    pthread_t pthread_receiveUdp,pthread_writeFile;
	FILE *fp = NULL;

	fp = fopen("udp.ts","a+");   
    if(!fp)
    {
        printf("Open udp.ts failed.\n");
    }
    
    ret = pthread_create(&pthread_receiveUdp, NULL, (void*)dj_udp_receive, NULL);
    if (ret)
    {
        printf("Create pthread error!\n");
        return 1;
    }
    
    ret = pthread_create(&pthread_writeFile, NULL, (void*)dj_udp_writeFile, fp);
    if (ret)
    {
        printf("Create pthread error!\n");
        return 1;
    }
    
    pthread_join(pthread_receiveUdp, NULL);
    pthread_join(pthread_writeFile, NULL);
    
    return 0;
}