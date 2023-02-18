#include <stdio.h>      /* perror */
#include <sys/socket.h> /* socket */
#include <arpa/inet.h>  /* inet */
#include <unistd.h>     /* close  */
#include <string.h>     /* memset */
#include <errno.h>      /* errno */
#include <stdlib.h>     /* malloc */
#include <sys/select.h> /* select */

#include "g_d_linked_list.h"
#include "d_linked_list_function.h"
#include "list_itr.h"
#include "net_server.h"

#define TRUE (1)
#define FALSE (0)
#define BUFFER_SIZE (4096)
#define MAX_PENDING (1000)

struct Server
{
    int m_activity;
    int m_hasToRun;
    void *m_context;
    NetServerERR m_errCode;
    Fail m_failFunc;
    CloseClient m_closeClient;
    NewClient m_newClient;
    GotMsg m_gotMsg;
    int m_backLog;
    int m_portNumber;
    int m_counter;
    fd_set *m_master;
    fd_set *m_temp;
    int m_length;
    int m_serverSocket;
    struct sockaddr_in m_address;
    List *m_dll;
};

struct DS_DATA
{
    void *m_msgContext;
    char *message;
    size_t m_messageLength;
    GotMsg m_gotMsg;
    int m_activity;
    int m_flag;
    fd_set *m_temp;
    fd_set *m_master;
    ListItr m_iter;
    char m_recieveBuff[BUFFER_SIZE];
    char m_sendBuff[BUFFER_SIZE]; /* need here ??? */
};

typedef struct DS_DATA DS_DATA;
static NetServerERR InitServerStruct(Server *_server, InitServerData *_serverData);
static NetServerERR InitServer(Server *server, struct sockaddr_in *address, int *socketServer);
static void DestroyFunc(void *_elem);
static NetServerERR AcceptNewClients(Server *_server);
static NetServerERR TryAcceptClient(Server *_server, int clientSocket);
static NetServerERR HandleAccepClientError(Server *_server, int _clientSocket, int errCode);
static void InitDataStructForEach(DS_DATA *dataForEach, Server *_server);
static int ExitForEach(void *_elem, void *_context);
static void HandleClientRequests(Server *_server);
static int ActionForEach(void *_elem, void *_context);
static int HandleRead(void *_elem, void *_context, int readBytes, char *buff);

Server *ServerCreate(InitServerData *_serverData)
{
    Server *server;
    if (_serverData == NULL || _serverData->m_gotMsg == NULL)
    {
        return NULL;
    }

    server = (Server *)malloc(sizeof(Server));
    if (server == NULL)
    {
        return NULL;
    }

    if (NET_SERVER_SUCCESS != InitServerStruct(server, _serverData))
    {
        free(server);
        return NULL;
    }
    server->m_master = malloc(sizeof(fd_set));
    server->m_temp = malloc(sizeof(fd_set));

    if ((server->m_errCode = InitServer(server, &server->m_address, &server->m_serverSocket)) != NET_SERVER_SUCCESS)
    {
        printf("i am here\n");
        if (server->m_failFunc != NULL)
        {
            server->m_failFunc(server->m_errCode, server->m_context);
        }

        ListDestroy(&(server->m_dll), NULL);
        free(server);
        return NULL;
    }

    return server;
}

void ServerDestroy(Server **_server)
{
    if (_server != NULL && *_server != NULL)
    {
        ListDestroy(&((*_server)->m_dll), DestroyFunc);
        close((*_server)->m_serverSocket);
        free(*_server);
        *_server = NULL;
    }
}

void StopRun(Server *_server)
{
    if (_server != NULL)
    {
        _server->m_hasToRun = FALSE;
    }
}

NetServerERR ServerRun(Server *_server)
{
    if (_server == NULL)
    {
        return SERVER_UNINITIALIZED;
    }
    FD_SET(_server->m_serverSocket, _server->m_master);

    while (_server->m_hasToRun)
    {
        if ((_server->m_errCode = AcceptNewClients(_server)) != NET_SERVER_SUCCESS)
        {
            if (_server->m_failFunc != NULL)
            {
                _server->m_failFunc(_server->m_errCode, _server->m_context);
            }

            return _server->m_errCode;
        }
        HandleClientRequests(_server);

        /*  sleep(1); */
    }

    return NET_SERVER_SUCCESS;
}

NetServerERR SendMsg(int _clientId, char *_msg, int _messageLen, void *context)
{
    int sendBytes = send(_clientId, _msg, _messageLen, 0);
    if (sendBytes < 0)
    {
        return SEND_FAIL;
    }

    return NET_SERVER_SUCCESS;
}

static void HandleClientRequests(Server *_server)
{
    DS_DATA dataForEach;
    ListItr end = ListItrEnd(_server->m_dll);

    InitDataStructForEach(&dataForEach, _server);

    while (!ListItrEquals(dataForEach.m_iter, end)) /* and activity bigger then 0... */
    {
        ListItrForEach(dataForEach.m_iter, end,
                       ActionForEach, &dataForEach);
        if (dataForEach.m_flag)
        {
            --_server->m_counter;
            ListItrRemove(ListItrPrev(dataForEach.m_iter));
        }
        dataForEach.m_flag = 0;
    }
}

static int ActionForEach(void *_elem, void *_context)
{
    if (FD_ISSET(*(int *)_elem, (((DS_DATA *)_context)->m_temp)))
    {
        int readBytes = recv(*(int *)_elem, ((DS_DATA *)_context)->m_recieveBuff,
                             sizeof(((DS_DATA *)_context)->m_recieveBuff), 0);

        return readBytes > 0 ? HandleRead(_elem, _context, readBytes, ((DS_DATA *)_context)->m_recieveBuff)
                             : ExitForEach(_elem, _context);
    }

    ((DS_DATA *)_context)->m_iter = ListItrNext(((DS_DATA *)_context)->m_iter);
    return 1;
}

static int HandleRead(void *_elem, void *_context, int readBytes, char *buff)
{
    int socketId = *(int *)_elem;
    ((DS_DATA *)_context)->m_gotMsg(((DS_DATA *)_context)->m_msgContext, socketId, buff, readBytes);

    ((DS_DATA *)_context)->m_iter = ListItrNext(((DS_DATA *)_context)->m_iter);
    return 1;
}

static int ExitForEach(void *_elem, void *_context)
{
    FD_CLR(*(int *)_elem, (((DS_DATA *)_context)->m_master));
    close(*(int *)_elem);
    free((int *)_elem);
    ((DS_DATA *)_context)->m_iter = ListItrNext(((DS_DATA *)_context)->m_iter);
    ((DS_DATA *)_context)->m_flag = 1;

    return 0;
}

static NetServerERR AcceptNewClients(Server *_server)
{
    int *elem;
    int clientSocket;

    *_server->m_temp = *_server->m_master;
    _server->m_activity = select(1024, _server->m_temp, NULL, NULL, NULL);
    if (FD_ISSET(_server->m_serverSocket, _server->m_temp))
    {
        /* activity--; server activity... */
        clientSocket = accept(_server->m_serverSocket, (struct sockaddr *)&_server->m_address,
                              (socklen_t *)&_server->m_length);
        if (clientSocket > 0)
        {
            return TryAcceptClient(_server, clientSocket);
        }
        else
        {
            perror("select error");
            return ACCEPT_FAIL;
        }
    }

    return NET_SERVER_SUCCESS;
}

static NetServerERR TryAcceptClient(Server *_server, int _clientSocket)
{
    /* all return success for server rutine, but real error defined in enum for user purposes. */
    int *elem;
    if (_server->m_counter == MAX_PENDING)
    {
        return HandleAccepClientError(_server, _clientSocket, MAX_PENDING_ERR);
    }

    elem = (int *)malloc(sizeof(int));
    if (elem == NULL)
    {
        return HandleAccepClientError(_server, _clientSocket, ALLOCATION_FAIL);
    }

    *elem = _clientSocket;
    if (ListPushTail(_server->m_dll, elem) != LIST_SUCCESS)
    {
        free(elem);
        return HandleAccepClientError(_server, _clientSocket, ALLOCATION_FAIL);
    }

    if (_server->m_newClient != NULL)
    {
        _server->m_newClient(_clientSocket, _server->m_context);
    }

    FD_SET(_clientSocket, _server->m_master);
    _server->m_counter++;

    return NET_SERVER_SUCCESS;
}

static NetServerERR HandleAccepClientError(Server *_server, int _clientSocket, int errCode)
{
    if (_server->m_closeClient)
    {
        _server->m_closeClient(_clientSocket, _server->m_context);
    }

    if (_server->m_failFunc != NULL)
    {
        _server->m_closeClient(errCode, _server->m_context);
    }

    close(_clientSocket);
    return NET_SERVER_SUCCESS;
}

static NetServerERR InitServer(Server *server, struct sockaddr_in *address, int *socketServer)
{
    int optval = 1;
    *socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (*socketServer < 0)
    {
        printf("1\n");
        return SERVER_SOCKET_FAIL;
    }

    if (setsockopt(*socketServer, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        printf("2\n");
        return REUSE_FAIL;
    }

    memset(address, 0, sizeof(*address));
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(server->m_portNumber);

    if (bind(*socketServer, (struct sockaddr *)address, sizeof(*address)) < 0)
    {
        return BIND_FAIL;
    }

    if (listen(*socketServer, server->m_backLog))
    {
        return LISTEN_FAIL;
    }

    FD_ZERO(server->m_master);
    FD_ZERO(server->m_temp);

    return NET_SERVER_SUCCESS;
}

static NetServerERR InitServerStruct(Server *_server, InitServerData *_serverData)
{
    int NetServerERRFail = ALLOCATION_FAIL;
    _server->m_portNumber = _serverData->m_portNumber;
    _server->m_backLog = _serverData->m_backLog;
    _server->m_gotMsg = _serverData->m_gotMsg;
    _server->m_newClient = _serverData->m_newClient;
    _server->m_closeClient = _serverData->m_closeClient;
    _server->m_failFunc = _serverData->m_failFunc;
    _server->m_context = _serverData->m_context;
    _server->m_hasToRun = TRUE;
    _server->m_errCode = NET_SERVER_SUCCESS;
    _server->m_length = sizeof(_server->m_address);
    _server->m_activity = 0;
    _server->m_dll = ListCreate();
    return _server->m_dll == NULL ? ALLOCATION_FAIL : NET_SERVER_SUCCESS;
}

static void InitDataStructForEach(DS_DATA *dataForEach, Server *_server)
{
    dataForEach->m_flag = 0;
    dataForEach->m_iter = ListItrBegin(_server->m_dll);
    dataForEach->m_master = _server->m_master;
    dataForEach->m_temp = _server->m_temp;
    dataForEach->m_activity = _server->m_activity;
    dataForEach->m_gotMsg = _server->m_gotMsg;
    dataForEach->m_msgContext = _server->m_context;
}

static void DestroyFunc(void *_elem)
{
    free((int *)_elem);
}