#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net_server.h"

static InitServerData *InitStruct(int m_portNumber, int m_backLog, GotMsg m_gotMsg, NewClient m_newClient,
                                  CloseClient m_closeClient, Fail m_failFunc,
                                  void *m_msgContext, void *m_newClientontext,
                                  void *m_closeClientContext, void *m_failContext);
static status GotMessage(void *_context, int _socketId, char *_buffPtr, int _buffSize);
static status fail(int _statusPtr, void *_context);
void testCreateBad1();
void testCreateBad2();
void testCreateGood();
void testCreateBadPort();
void testCreateBadPort();
void runServer();

int main()
{
    runServer();
    /* testCreateBad1();
    testCreateBad2();
    testCreateGood();
    testCreateBadPort(); */

    return 0;
}

void runServer()
{
    InitServerData *data = InitStruct(6060, 1000, GotMessage, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    Server *server = ServerCreate(data);
    if (server == NULL)
    {
        printf("fail...\n");
    }
    else
    {
        printf("success\n");
    }

    ServerRun(server);
}

void testCreateBadPort()
{
    InitServerData *data = InitStruct(9000, 1000, GotMessage, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    InitServerData *data2 = InitStruct(9000, 1000, GotMessage, NULL, NULL, fail, NULL, NULL, NULL, NULL);
    Server *server = ServerCreate(data);
    Server *server2 = ServerCreate(data2);

    printf("test create bad port\n");

    if (server2 != NULL)
    {
        printf("\033[31mFAIL\033[0m\n");
    }

    free(data);
    free(data2);
    ServerDestroy(&server);
}

void testCreateGood()
{
    InitServerData *data = InitStruct(5050, 1000, GotMessage, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    Server *server = ServerCreate(data);
    printf("test create good\n");
    if (server != NULL)
    {
        printf("\033[32mPASS\033[0m\n");
    }
    else
    {
        printf("\033[31mFAIL\033[0m\n");
    }

    free(data);
    ServerDestroy(&server);
}

void testCreateBad2()
{
    InitServerData *data = InitStruct(5050, 1000, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    printf("test create null got message\n");
    if (ServerCreate(data) == NULL)
    {
        printf("\033[32mPASS\033[0m\n");
    }
    else
    {
        printf("\033[31mFAIL\033[0m\n");
    }

    free(data);
}

void testCreateBad1()
{
    printf("test create null\n");

    if (ServerCreate(NULL) == NULL)
    {
        printf("\033[32mPASS\033[0m\n");
    }
    else
    {
        printf("\033[31mFAIL\033[0m\n");
    }
}

static InitServerData *InitStruct(int m_portNumber, int m_backLog, GotMsg m_gotMsg, NewClient m_newClient,
                                  CloseClient m_closeClient, Fail m_failFunc,
                                  void *m_msgContext, void *m_newClientontext,
                                  void *m_closeClientContext, void *m_failContext)
{
    InitServerData *data = (InitServerData *)malloc(sizeof(InitServerData));

    data->m_gotMsg = m_gotMsg;
    data->m_portNumber = m_portNumber;
    data->m_backLog = m_backLog;
    data->m_newClient = m_newClient;
    data->m_closeClient = m_closeClient;
    data->m_failFunc = m_failFunc;
    data->m_msgContext = m_msgContext;
    data->m_newClientontext = m_newClientontext;
    data->m_closeClientContext = m_closeClientContext;
    data->m_failContext = m_failContext;
    /*  strcpy(data->m_message, "hello from server");
     data->messageLength = strlen(data->m_message); */
    return data;
}

static status GotMessage(void *_context, int _socketId, char *_buffPtr, int _buffSize)
{
    char *msg = "hello from server";
    /* clientid, message,messagelen, context buff -->   */
    SendMsg(_socketId, msg, strlen(msg), _context);
    printf("client id from got message = %d\n", _socketId);
    return 1;
}

static status fail(int _statusPtr, void *_context)
{
    if (_statusPtr == BIND_FAIL)
    {
        printf("\033[32mPASS\033[0m\n");
    }
    else
    {
        printf("\033[31mFAIL\033[0m\n");
    }
    return _statusPtr;
}
