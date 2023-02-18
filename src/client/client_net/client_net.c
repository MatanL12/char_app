#include "client_net.h"
#include "limitations.h"
#include <arpa/inet.h>  /* struct sockaddr_in */
#include <sys/socket.h> /* socket() */
#include <string.h>     /* memset() */
#include <stdio.h>      /* stdio */
#include <unistd.h>     /* close */

ClientNetERR ClientInit(int *_socket, int _port, char *_ip)
{
    struct sockaddr_in sinClient;
    int tempSock;

    if (_socket == NULL)
    {
        return CLIENT_NET_NOT_INITIALIZED;
    }

    tempSock = socket(AF_INET, SOCK_STREAM, 0);

    if (tempSock < 0)
    {
        perror("sockClient failed\n");
        return CLIENT_NET_CONNECT_ERR;
    }

    memset(&sinClient, 0, sizeof(sinClient));
    sinClient.sin_family = AF_INET;
    sinClient.sin_addr.s_addr = inet_addr(_ip);
    sinClient.sin_port = htons(_port);

    if (connect(tempSock, (struct sockaddr *)&sinClient, sizeof(sinClient)) < 0)
    {
        close(tempSock);
        perror("connection failed\n");
        return CLIENT_NET_CONNECT_ERR;
    }

    *_socket = tempSock;

    return CLIENT_NET_SUCCESS;
}

ClientNetERR ClientClose(int _socket)
{
    if (_socket < MIN_SOCK_NUM || _socket > MAX_PORT_NUM)
    {
        return CLIENT_NET_ID_ERR;
    }

    close(_socket);

    return CLIENT_NET_SUCCESS;
}

ClientNetERR ClientSend(int _socket, void *_bufferSend, size_t _bufferSize)
{
    ssize_t sendBytes;

    if (_bufferSend == NULL)
    {
        return CLIENT_NET_NOT_INITIALIZED;
    }

    if (_socket < MIN_SOCK_NUM || _socket > MAX_SOCK_NUM)
    {
        return CLIENT_NET_ID_ERR;
    }

    sendBytes = send(_socket, _bufferSend, _bufferSize, 0);
    if (sendBytes < 0)
    {
        perror("send to server failed\n");
        return CLIENT_NET_SEND_ERR;
    }

    return CLIENT_NET_SUCCESS;
}

ClientNetERR ClientRecv(int _socket, void *_bufferReceive)
{
    ssize_t sendBytes, readBytes;

    if (_bufferReceive == NULL)
    {
        return CLIENT_NET_NOT_INITIALIZED;
    }

    if (_socket < MIN_SOCK_NUM || _socket > MAX_SOCK_NUM)
    {
        return CLIENT_NET_ID_ERR;
    }

    readBytes = recv(_socket, _bufferReceive, MAX_BUFFER_LENGTH, 0);
    if (readBytes <= 0)
    {
        close(_socket);
        perror("send from server failed\n");
        return CLIENT_NET_RECV_ERR;
    }

    return CLIENT_NET_SUCCESS;
}
