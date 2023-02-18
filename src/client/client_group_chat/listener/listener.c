#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* pid */
#include <time.h>
#include <stdio.h> /* printf */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "limitations.h"
#include "data_net.h"

#define MSGBUFSIZE 256



int main(int argc, char *argv[])
{
    char groupIP[MAX_IP_LEN];
    char groupName[MAX_GROUP_NAME_SIZE];
    int groupPort, fd, recvBytes;
    struct sockaddr_in addr;
    struct ip_mreq mreq;
    socklen_t addrlen = sizeof(addr);
    socklen_t addr2len;
    struct tm *ptm;
    char msgbuf[MSGBUFSIZE];
    int optval = 1;
    time_t rawtime = time(NULL);

    /* write the PID into a file  */
    FILE *fp = fopen("ListenerPIDsToKill.txt", "w");

    if (NULL == fp)
    {
        perror("cannot open the file");
        return 1;
    }

    fprintf(fp, "%d\n", getpid());
    fclose(fp);

    /* extract IP PORT and Group name */
    strcpy(groupIP, argv[1]);
    groupPort = atoi(argv[2]);
    strcpy(groupName, argv[3]);


    /* get current time  */
    if (rawtime == -1)
    {
        puts("The time() function failed");
        return 1;
    }

    ptm = localtime(&rawtime);

    if (ptm == NULL)
    {
        puts("The localtime() function failed");
        return 1;
    }

    /* create UDP socket */
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket failed");
        return 1;
    }

    /* allow re-using for the socket */
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) < 0)
    {
        perror("Reusing ADDR failed");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(groupPort);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind failed");
        return 1;
    }

    /* mulitcast */
    mreq.imr_multiaddr.s_addr = inet_addr(groupIP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    addr2len = sizeof(mreq);

    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, addr2len) < 0)
    {
        perror("setsockopt failed");
        return 1;
    }

    printf("%s \n\n", groupName);
    while (1)
    {
        recvBytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *)&addr, &addrlen);
        if (recvBytes < 0)
        {
            perror("recvfrom failed");
            return 1;
        }

        msgbuf[recvBytes] = '\0';

        rawtime = time(NULL);
        ptm = localtime(&rawtime);

        printf("%s", msgbuf);
        printf(" %02d:%02d:%02d\n\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    }

    return 0;
}

