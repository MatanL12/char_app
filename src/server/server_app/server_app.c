#include <string.h> /* strcpy */
#include <stdlib.h>
#include <stdio.h>

#include "net_server.h"
#include "data_net.h"
#include "limitations.h"
#include "user_manager.h"
#include "net_server.h"
#include "server_app.h"
#include "group_manager.h"
#include "g_vector.h"
#include "protocol.h"

#define FAIL -1
#define OK (0)

typedef struct Context Context;

struct Context
{
    GroupManager *m_groupManager;
    UserManager *m_manager;
    char m_userName[MAX_NAME_LENGTH];
    char m_password[MAX_PASSWORD_LENGTH];
    char m_groupName[MAX_GROUP_NAME_SIZE];
    char m_port[PORT_SIZE];
    char m_ip[MAX_IP_LEN];
    char *m_groupResponse;
};

struct ServerAppManager
{
    Server *m_serverNet;
    Context *m_context;
};
/* ------------------------------ Static Functions - Declaration ------------------------------ */

/* for debug */
/* void TestPrintUserList(UserManager *_userManager, char *username); */

/* functions required for init the server */
static void InitData(InitServerData *_serverData, Context *_gotMsgContext, int _port);
static Context *SetUpContext(Context *_context);

/* got message function */
static NetServerERR GotMsgFunc(void *_context, int _socketId, char *_buffPtr, int _buffSize);

/* decide which handler to invoke by tag response.*/
static int HandleReqControl(int _socketId, char *_buffPtr, int _buffSize, void *_context);

/* request handlers -> unpack , pack response and send message to client  */
typedef int (*handleReqPtr)(char *buff, size_t _messageLen, Context *context, int sockId);
static int dummy(char *buff, size_t _messageLen, Context *context, int sockId);
static int HandleRegistration(char *buff, size_t _messageLen, Context *context, int sockId);
static int HandleLogin(char *buff, size_t _messageLen, Context *context, int sockId);
static int HandleAddGroup(char *_clientBuff, size_t _messageLen, Context *context, int sockId);
static int HandleRenderGroups(char *_clientBuff, size_t _messageLen, Context *_context, int _sockId);
static int HandleJoinGroup(char *_clientBuff, size_t _messageLen, Context *_context, int _sockId);
static int HandleLeaveGroup(char *_clientBuff, size_t _messageLen, Context *_context, int _sockId);
static int HandleLogOut(char *_clientBuff, size_t _messageLen, Context *_context, int _sockId);

/* action functions */
static void UserRegistration(int _socketId, void *_context, char *_msg);
static void UserLogin(int _socketId, void *_context, char *_msg);
static int ServerCreateGroup(int sockId, Context *context);
static int UserJoinGroup(Context *_context);
static int UserLeaveGroup(Context *_context, char *_msg);
static int LogOutAction(Context *_context, char *msg);
static void DestroyContext(Context *_context);

/* ---------------------------------- API ---------------------------------- */
ServerAppManager *CreateAppManager(int _port)
{
    ServerAppManager *serverApp;
    InitServerData serverData;
    Context *context;

    if(_port > MAX_PORT_NUM || _port < MIN_PORT_NUM)
    {
        return NULL;
    }

    serverApp = (ServerAppManager *)malloc(sizeof(ServerAppManager));
    if (serverApp == NULL)
    {
        return NULL;
    }
    serverApp->m_context = (Context *)malloc(sizeof(Context));
    if (serverApp->m_context == NULL)
    {
        free(serverApp);
        return NULL;
    }

    serverApp->m_context = SetUpContext(serverApp->m_context);
    if (serverApp->m_context == NULL)
    {
        free(serverApp->m_context);
        free(serverApp);
        return NULL;
    }

    InitData(&serverData, serverApp->m_context, _port);
    serverApp->m_serverNet = ServerCreate(&serverData);

    if (serverApp->m_serverNet == NULL)
    {
        DestroyUserMan(&(serverApp->m_context->m_manager));
        DestroyGroupManager(serverApp->m_context->m_groupManager);
        free(serverApp->m_context);
        free(serverApp);

        return NULL;
    }

    return serverApp;
}

void DestroyAppManager(ServerAppManager **_appManager)
{
    if (_appManager != NULL && *_appManager != NULL)
    {
        DestroyContext((*_appManager)->m_context);
        ServerDestroy(&((*_appManager)->m_serverNet));
        free(*_appManager);
        *_appManager = NULL;
    }
}

ServerAppErr ServerManagerRun(ServerAppManager *_appManager)
{
    if (_appManager == NULL)
    {
        return SERVER_APP_UNINITIALIZED;
    }

    if (ServerRun(_appManager->m_serverNet) != NET_SERVER_SUCCESS)
    {
        return SERVER_APP_SERVER_RUN_FAILED;
    }

    return SERVER_APP_SUCCESS;
}

/***************************** initialization *************************************/
static void InitData(InitServerData *_serverData, Context *_gotMsgContext, int _port)
{
    _serverData->m_portNumber = _port;
    _serverData->m_backLog = BACK_LOG;
    _serverData->m_gotMsg = GotMsgFunc;
    _serverData->m_failFunc = NULL;
    _serverData->m_newClient = NULL;
    _serverData->m_closeClient = NULL;
    _serverData->m_context = _gotMsgContext;
}

static Context *SetUpContext(Context *_context)
{
    if ((_context->m_manager = CreateUserMan()) == NULL)
    {
        return NULL;
    }

    if ((_context->m_groupManager = CreateGroupManager()) == NULL)
    {
        DestroyUserMan(&(_context->m_manager));
        return NULL;
    }

    return _context;
}

static NetServerERR GotMsgFunc(void *_context, int _socketId, char *_clientBuff, int _buffSize)
{
    HandleReqControl(_socketId, _clientBuff, _buffSize, _context);
}

/**************************** handlers managment ********************************/
static int HandleReqControl(int _socketId, char *_clientBuff, int _buffSize, void *_context)
{
    handleReqPtr arrHandle[RESPONSETYPE_NUM_OF_RESPONSES] = {&dummy, &HandleRegistration,
                                                       &HandleLogin, &HandleAddGroup,
                                                       &HandleRenderGroups, &HandleJoinGroup,
                                                       &HandleLeaveGroup, &HandleLogOut};
    Tags tagResult = RetrunTag(_clientBuff);
    arrHandle[tagResult](_clientBuff, _buffSize, _context, _socketId);
}

static int HandleRegistration(char *_clientBuff, size_t _messageLen, Context *_context, int _sockId)
{
    size_t msgLen;
    char buffer[MAX_BUFFER_LENGTH], msg;

    UnPackRegistration(_clientBuff, _messageLen, ((Context *)_context)->m_userName,
                       ((Context *)_context)->m_password);
    UserRegistration(_sockId, _context, &msg);
    msgLen = PackResponseReg(buffer, msg);
    SendMsg(_sockId, buffer, msgLen, _context);
}

static int HandleLogin(char *buff, size_t _messageLen, Context *_context, int _sockId)
{
    size_t msgLen;
    char buffer[MAX_BUFFER_LENGTH], msg;
    UnPackLogin(buff, _messageLen, ((Context *)_context)->m_userName,
                ((Context *)_context)->m_password);
    UserLogin(_sockId, _context, &msg);
    msgLen = PackResponseLogin(buffer, msg);
    SendMsg(_sockId, buffer, msgLen, _context);
}

static int HandleAddGroup(char *_clientBuff, size_t _messageLen, Context *_context, int _sockId)
{
    char buff[MAX_BUFFER_LENGTH];
    size_t msgLen;
    UnPackCreateGroup(_clientBuff, _messageLen, _context->m_userName, _context->m_groupName);
    ServerCreateGroup(_sockId, _context);
    msgLen = PackResponseCreateGroup(buff, _context->m_ip, _context->m_port);
    SendMsg(_sockId, buff, msgLen, _context);
}

static int HandleRenderGroups(char *_clientBuff, size_t _messageLen, Context *_context, int _sockId)
{
    char buff[MAX_BUFFER_LENGTH];
    Vector *activeGroupsVec;
    size_t msgLen, numOfGroups;

    numOfGroups = GetNumOfGroups(((Context *)_context)->m_groupManager);

    activeGroupsVec = VectorCreate(numOfGroups, 0);

    GetGroupNames(((Context *)_context)->m_groupManager, activeGroupsVec);
    msgLen = PackRenderResponse(buff, activeGroupsVec);
    SendMsg(_sockId, buff, msgLen, _context);

    /* TBD - to differ between 2 cases when the num of elements in the vector is 0:
    (1)allocation failure; (2) no active groups -  */

    VectorDestroy(&activeGroupsVec, NULL);

    return OK;
}

static int HandleJoinGroup(char *_clientBuff, size_t _messageLen, Context *_context, int _sockId)
{
    char buff[MAX_BUFFER_LENGTH];
    size_t msgLen;

    UnPackJoinGroup(_clientBuff, _messageLen, _context->m_userName, _context->m_groupName);
    UserJoinGroup(_context);
    msgLen = PackResponseJoinGroup(buff, _context->m_ip, _context->m_port);
    SendMsg(_sockId, buff, msgLen, _context);
}

static int HandleLeaveGroup(char *_clientBuff, size_t _messageLen, Context *_context, int _sockId)
{
    char msg;
    char buff[MAX_BUFFER_LENGTH];
    size_t msgLen;
    UnPackLeaveGroup(_clientBuff, msgLen, _context->m_userName, _context->m_groupName);
    UserLeaveGroup(_context, &msg);
    msgLen = PackResponseLeaveGroup(buff, msg);
    SendMsg(_sockId, buff, msgLen, _context);
}

static int HandleLogOut(char *_clientBuff, size_t _messageLen, Context *_context, int _sockId)
{
    size_t msgLen;
    char msg;
    char buff[MAX_BUFFER_LENGTH];
    UnPackLogout(_clientBuff, _messageLen, _context->m_userName);
    LogOutAction(_context, &msg);
    msgLen = PackResponseLogout(buff, msg);
    SendMsg(_sockId, buff, msgLen, _context);
}

/********************************* action function ***************************************/
static void UserRegistration(int _socketId, void *_context, char *_msg)
{
    if (AddNewUser(((Context *)_context)->m_manager,
                   ((Context *)_context)->m_userName,
                   ((Context *)_context)->m_password) != SUCCESS_USER_MANAGER)
    {
        *_msg = RESPONSETYPE_REGISTRAION_FAILED;
    }
    else
    {
        *_msg = RESPONSETYPE_REGISTRAION_SUCCESS;
    }
}

static void UserLogin(int _socketId, void *_context, char *_msg)
{
    if (LoginUser(((Context *)_context)->m_manager, ((Context *)_context)->m_userName,
                  ((Context *)_context)->m_password) != SUCCESS_USER_MANAGER)
    {
        *_msg = RESPONSETYPE_LOGIN_FAILED;
    }
    else
    {
        *_msg = RESPONSETYPE_LOGIN_SUCCES;
    }

    return;
}

static int ServerCreateGroup(int sockId, Context *context)
{
    GroupManagerErr errCode;
    statusUserManager errUserCode;
    errCode = AddGroup(context->m_groupManager, context->m_groupName, context->m_ip,
                       context->m_port, &(context->m_groupResponse));
    if (errCode != GROUP_MANAGER_SUCCESS)
    {
        printf("ip fail - %s\n", context->m_ip);
        strcpy(context->m_ip, "0");
        return FAIL;
    }

    errUserCode = InserGroupToUser(context->m_manager, context->m_userName,
                                   context->m_groupResponse);
    if (errUserCode != SUCCESS_USER_MANAGER)
    {
        strcpy(context->m_ip, "0");
        printf("fail\n");
        return FAIL;
    }

    return OK;
}

static int UserJoinGroup(Context *_context)
{
    GroupManagerErr errCode;
    statusUserManager errUserCode;

    if (HasJoinedManager(_context->m_manager, _context->m_userName, _context->m_groupName))
    {
        strcpy(_context->m_ip, "0");
        printf("already joined\n");
        return FAIL;
    }

    errCode = JoinGroup(_context->m_groupManager, _context->m_groupName,
                        _context->m_ip, _context->m_port, &(_context->m_groupResponse));

    if (errCode != GROUP_MANAGER_SUCCESS)
    {
        strcpy(_context->m_ip, "0");
        printf("ip fail - %s\n", _context->m_ip);
        return FAIL;
    }

    errUserCode = InserGroupToUser(_context->m_manager, _context->m_userName,
                                   _context->m_groupResponse);
    if (errUserCode != SUCCESS_USER_MANAGER)
    {
        LeaveGroup(_context->m_groupManager, _context->m_groupName);
        strcpy(_context->m_ip, "0");
        printf("ip fail - %s\n", _context->m_ip);
        return FAIL;
    }

    return OK;
}

static int UserLeaveGroup(Context *_context, char *_msg)
{
    if (LeaveGroupUserManagment(_context->m_manager, _context->m_userName, _context->m_groupName) !=
        SUCCESS_USER_MANAGER)
    {
        *_msg = RESPONSETYPE_LEAVE_GROUP_FAIL;
        return FAIL;
    }
    if (LeaveGroup(_context->m_groupManager, _context->m_groupName) != GROUP_MANAGER_SUCCESS)
    {
        *_msg = RESPONSETYPE_LEAVE_GROUP_FAIL;
        return FAIL;
    }

    *_msg = RESPONSETYPE_LEAVE_GROUP_SUCCESS;

    return OK;
}

static int LogOutAction(Context *_context, char *msg)
{
    char buffGroup[MAX_BUFFER_LENGTH];
    size_t i = 0;
    size_t userNumOfGroups;
    if (!IsUserLoggedUserManager(_context->m_manager, _context->m_userName))
    {
        *msg = RESPONSE_LOG_OUT_FAIL;
        return FAIL;
    }

    userNumOfGroups = GetNumOfUserGroupsUserManager(_context->m_manager, _context->m_userName);

    for (; i < userNumOfGroups; i++)
    {
        RemoveUserGroup(_context->m_manager, _context->m_userName, buffGroup);
        LeaveGroup(_context->m_groupManager, buffGroup);
    }

    LogoutUser(_context->m_manager, _context->m_userName);
    *msg = RESPONSE_LOG_OUT_SUCCESS;

    return OK;
}

static void DestroyContext(Context *_context)
{
    DestroyUserMan(&(_context->m_manager));
    free(_context);
}

NetServerERR NewClientFunc(int _socketId, void *_context)
{
}

NetServerERR CloseClientFunc(int _socketId, void *_context)
{
}

NetServerERR FailFunc(int _statusPtr, void *_context)
{
}

/* dummy function. */
static int dummy(char *buff, size_t _messageLen, Context *context, int sockId)
{
    (void)buff;
    (void)_messageLen;
    (void)context;
    (void)sockId;

    return 0;
}
