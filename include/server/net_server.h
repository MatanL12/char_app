#ifndef __NETSERVER_H__
#define __NETSERVER_H__

typedef struct Server Server;

typedef enum NetServerERR
{
    NET_SERVER_SUCCESS,
    SERVER_SOCKET_FAIL,
    REUSE_FAIL,
    BIND_FAIL,
    LISTEN_FAIL,
    NON_BLOCKING_FAIL,
    ACCEPT_FAIL,
    DL_FAIL,
    SERVER_UNINITIALIZED,
    SELECT_ERROR,
    ALLOCATION_FAIL,
    MAX_PENDING_ERR,
    SEND_FAIL
} NetServerERR;

typedef struct InitServerData InitServerData;

typedef NetServerERR (*GotMsg)(void *_context, int _socketId, char *_buffPtr, int _buffSize);
typedef NetServerERR (*NewClient)(int _socketId, void *_context);
typedef NetServerERR (*CloseClient)(int _socketId, void *_context);
typedef NetServerERR (*Fail)(int _NetServerERRPtr, void *_context);

struct InitServerData
{
    int m_portNumber;
    int m_backLog;
    GotMsg m_gotMsg;
    NewClient m_newClient;
    CloseClient m_closeClient;
    Fail m_failFunc;
    void *m_context;
};

Server *ServerCreate(InitServerData *_serverDataInit);
void ServerDestroy(Server **_server);
NetServerERR ServerRun(Server *_server);

/*by the got message*/
void StopRun(Server *_server);
NetServerERR SendMsg(int _clientId, char *_msg, int _messageLen, void *context);
/* send message from the application... */

#endif /* __SERVERSELECT__ */