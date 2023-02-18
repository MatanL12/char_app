#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>    /* pid */
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "limitations.h"
#include "data_net.h"


# define MAX_MESSAGE_LENGTH 4000



int main(int argc, char *argv[])
{
    char groupIP[MAX_IP_LEN];
    char userName[MAX_GROUP_NAME_SIZE];
    char groupName[MAX_GROUP_NAME_SIZE];
    int groupPort, fd , sentBytes;
    struct sockaddr_in addr;
    const int delay_secs = 1;
    socklen_t  addrlen = sizeof(addr);
    char message[50];
    size_t msgLen;

     /* write the PID into a file  */
    FILE *fp = fopen("SenderPIDsToKill.txt","w");

    if (NULL == fp)
    {
        perror("cannot open the file");
        return 1;
    }

    fprintf(fp, "%d\n",getpid());
    fclose(fp);
    
    /* extract IP PORT  Group name and User name*/
    strcpy(groupIP, argv[1]);
    groupPort = atoi(argv[2]);
    strcpy(groupName, argv[3]);
    strcpy(userName, argv[4]);

    /* create UDP socket */ 
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) 
    {
        perror("socket failed");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(groupIP);
    addr.sin_port = htons(groupPort);
    
    printf("Hello %s,\nWelcome to %s \n\n",userName,groupName);
    while (1) 
    {
        
        fgets(message,MAX_MESSAGE_LENGTH,stdin);

        strcat(message, userName);
        
        msgLen = strlen(message);
       
        if (msgLen > MAX_MESSAGE_LENGTH)
        {
            printf("Message excceds maximum size");
            continue;
        }

        sentBytes = sendto(fd, message, msgLen,0,(struct sockaddr*) &addr,sizeof(addr));
        if (sentBytes < 0) 
        {
            perror("sendto failed");
            return 1;
        }

  
     }

    return 0;
}