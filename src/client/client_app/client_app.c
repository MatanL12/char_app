#include <stdlib.h>    /* size_t */
#include <string.h>    /* strcpy, strcmp */
#include <stdio.h>     /* printf */
#include <sys/types.h> /* pid */
#include <signal.h>    /* kill */
#include <sys/types.h> /* kill */

#include "client_net.h"
#include "data_net.h"
#include "limitations.h"
#include "client_app.h"
#include "protocol.h"
#include "clientUI.h"
#include "g_d_linked_list.h"
#include "g_vector.h"
#include "d_linked_list_function.h"
#include "clientGroupChat.h"

#define MAX_INPUT_FROM_USER 5
#define MAGIC_NUMBER 942315
#define OK 1
#define VALID_INPUT 0
#define INVALID_INPUT -1
#define ESCAPE_PRESSED ("27")
#define EQUAL 0
#define VECTOR_BLOCK_SIZE (10) /* optional for future needs. */

typedef struct ConnectionData ConnectionData;

typedef struct GroupData GroupData;

typedef enum
{
    REGISTRATION = 1,
    LOGIN,
    EXIT
} MAIN_MANU;

typedef enum
{
    CREATE_GROUP = 1,
    JOIN_GROUP,
    LEAVE_GROUP,
    LOG_OUT
} LOGIN_MENU;

typedef enum
{
    EXIT_MANU,
    MAIN_SCREEN,
    LOGGED_SCREEN
} Screen;

struct ClientApp
{
    int m_socket;
    char m_userName[MAX_NAME_LENGTH];
    char m_password[MAX_PASSWORD_LENGTH];
    List *m_listGroupNames;
    char m_bufferReceive[MAX_BUFFER_LENGTH];
    size_t bufferSize;
    int m_magicNumber;
};

struct GroupData
{
    char m_groupName[MAX_GROUP_NAME_SIZE];
    pid_t m_pidListener;
    pid_t m_pidSender;
};

struct ConnectionData
{
    char port[PORT_SIZE];
    char ip[MAX_IP_LEN];
    GroupData *m_groupData;
};

typedef void (*HandleFunc)(int *_manu, ClientApp *_clientApp, void *_context);

static int MainScreen();
static void HandleMainScreenMenu(int *mane, ClientApp *_clientApp);
static int LoginScreen();
static void HandleLoggedScreen(int *_manu, ClientApp *_clientApp);

static ClientAppERR MainMenuAction(ClientApp *_clientApp, int _action, int *mane);
static ClientAppERR CreateNewGroup(int *_manu, ClientApp *_clientApp);
static ClientAppERR RenderGroupsToScreen(int *_manu, ClientApp *_clientApp);
static ClientAppERR ClientJoinGroup(int *_mane, ClientApp *_clientApp, Vector *_vector);
static ClientAppERR LeaveGroup(int *_manu, ClientApp *_clientApp);
static ClientAppERR LogOut(int *_manu, ClientApp *_clientApp);

static ClientAppERR SendRecvToNetServer(ClientApp *_clientApp, char *_buffSend);
static ClientAppERR AddGroupToList(ClientApp *_clientApp, char *_groupName, int *_manu, GroupData **_groupData);
static void RemoveGroupFromList(ClientApp *_clientApp);

/* Response handlers */
static ClientAppERR HandleFuncsControl(char _msg, int *_mane, ClientApp *_clientApp, void *_context);
static void HandleLoginFail(int *_mane, ClientApp *_clientApp, void *_context);
static void HandleLoginSuccess(int *_mane, ClientApp *_clientApp, void *_context);
static void HandleRegSuccess(int *_mane, ClientApp *_clientApp, void *_context);
static void HandleRegFail(int *_mane, ClientApp *_clientApp, void *_context);
static void HandleCreateGroupFail(int *_mane, ClientApp *_clientApp, void *_context);
static void HandleCreateGroupSuccess(int *_mane, ClientApp *_clientApp, void *_context);
static void HandleRenderGroupFail(int *_mane, ClientApp *_clientApp, void *_context);
static void HandleRenderGroupSuccess(int *_mane, ClientApp *_clientApp, void *_context);
static void handleJoinGroupFail(int *_mane, ClientApp *_clientApp, void *_context);
static void HandleJoinGroupSuccess(int *_mane, ClientApp *_clientApp, void *_context);
static void HandleLeaveGroup(int *_mane, ClientApp *_clientApp, void *_context);
static void HandleLogoutSuccess(int *_mane, ClientApp *_clientApp, void *_context);

/* Input validation  */
static void GetUsername(ClientApp *_clientApp);
static void GetUserPassword(ClientApp *_clientApp);
static int IsValidInput(char *_userInputString, int *_userInputInt, int (*InputValidationFunc)(int _input, void *_context), void *_context);
static int InputValidationMainScreen(int _input, void *_context);
static int InputValidationLoginScreen(int _input, void *_context);
static int InputValidationJoinGroup(int _input, void *_context);
static int InputValidationLeaveGroup(int _input, void *_context);
static int ParseInput(char *_input, int *_inputLen);

static int RenderGroupsToForEach(void *_element, size_t _index, void *_context);
static void DestroyVector(void *_item);
static int PrintListAction(void *_element, void *_context);
static void ListGroupDestroyItem(void *_element);

/* ------------------------------- API ------------------------------- */

ClientApp *CreateClientApp(int _port)
{
    ClientApp *ptrClientApp;

    if (_port > MAX_PORT_NUM || _port < MIN_PORT_NUM)
    {
        return NULL;
    }

    if ((ptrClientApp = (ClientApp *)malloc(sizeof(ClientApp))) == NULL)
    {
        return NULL;
    }

    if ((ptrClientApp->m_listGroupNames = ListCreate()) == NULL)
    {
        free(ptrClientApp);
        return NULL;
    }

    if (ClientInit(&(ptrClientApp->m_socket), _port, IP) != CLIENT_NET_SUCCESS)
    {
        ListDestroy(&(ptrClientApp->m_listGroupNames), NULL);
        free(ptrClientApp);
        return NULL;
    }

    ptrClientApp->m_magicNumber = MAGIC_NUMBER;

    return ptrClientApp;
}

ClientAppERR RunClientApp(ClientApp *_clientApp)
{
    int manu = MAIN_SCREEN;
    int userInput;
    char msg[2];

    if (_clientApp == NULL)
    {
        return CLIENT_APP_NOT_INITIALIZED;
    }

    system("clear");

    while (manu != EXIT_MANU)
    {
        switch (manu)
        {
        case MAIN_SCREEN:
            HandleMainScreenMenu(&manu, _clientApp);
            break;

        case LOGGED_SCREEN:
            HandleLoggedScreen(&manu, _clientApp);
            break;
        }
    }

    system("clear");

    PrintExit();

    return CLIENT_APP_SUCCESS;
}

void DestroyClientApp(ClientApp *_clientApp)
{
    if (_clientApp != NULL && _clientApp->m_magicNumber == MAGIC_NUMBER)
    {
        ClientClose(_clientApp->m_socket);
        ListDestroy(&(_clientApp->m_listGroupNames), ListGroupDestroyItem);
        _clientApp->m_magicNumber = 0;
        free(_clientApp);
    }
}

/* --------------------------- Static Functions --------------------------- */

static void HandleMainScreenMenu(int *mane, ClientApp *_clientApp)
{
    int userInput = MainScreen();

    if (userInput == EXIT)
    {
        *mane = EXIT_MANU;
        return;
    }

    system("clear");

    GetUsername(_clientApp);
    GetUserPassword(_clientApp);

    system("clear");

    MainMenuAction(_clientApp, userInput, mane);
}

static int MainScreen()
{
    int state;
    char userInput[MAX_INPUT_FROM_USER];

    PrintMainScreen(userInput);

    while ((IsValidInput(userInput, &state, InputValidationMainScreen, NULL)) == INVALID_INPUT)
    {
        PrintInvalidInput();
        PrintMainScreen(userInput);
    }

    return state;
}

static void HandleLoggedScreen(int *_manu, ClientApp *_clientApp)
{
    int userState = LoginScreen();

    system("clear");

    switch (userState)
    {
    case CREATE_GROUP:
        CreateNewGroup(_manu, _clientApp);
        break;

    case JOIN_GROUP:
        RenderGroupsToScreen(_manu, _clientApp);
        break;

    case LEAVE_GROUP:
        LeaveGroup(_manu, _clientApp);
        break;

    case LOG_OUT:
        LogOut(_manu, _clientApp);
        break;
    }
}

static int LoginScreen()
{
    int state;
    char userInput[MAX_INPUT_FROM_USER];

    PrintLoginScreen(userInput);

    while ((IsValidInput(userInput, &state, InputValidationLoginScreen, NULL)) == INVALID_INPUT)
    {
        PrintInvalidInput();
        PrintLoginScreen(userInput);
    }

    return state;
}

/* --------------------------------- User Action Function --------------------------------- */

static ClientAppERR MainMenuAction(ClientApp *_clientApp, int _action, int *mane)
{
    char bufferSend[MAX_BUFFER_LENGTH], msgResponse;
    size_t messageLen;
    ClientAppERR status;

    switch (_action)
    {
    case REGISTRATION:
        _clientApp->bufferSize = PackRegistration(bufferSend,
                                                  _clientApp->m_userName, _clientApp->m_password);
        break;
    case LOGIN:
        _clientApp->bufferSize = PackLogin(bufferSend, _clientApp->m_userName,
                                           _clientApp->m_password);
        break;
    }

    if ((status = SendRecvToNetServer(_clientApp, bufferSend)) != CLIENT_APP_SUCCESS)
    {
        PrintFailServer();
        return status;
    }

    switch (_action)
    {
    case REGISTRATION:
        UnPackResponseReg(_clientApp->m_bufferReceive, messageLen, &msgResponse);
        break;
    case LOGIN:
        UnPackResponseLogin(_clientApp->m_bufferReceive, messageLen, &msgResponse);
        break;
    }

    return HandleFuncsControl(msgResponse, mane, _clientApp, NULL);
}

static ClientAppERR CreateNewGroup(int *_manu, ClientApp *_clientApp)
{
    size_t messageLen;
    char bufferGroupName[MAX_GROUP_NAME_SIZE + 1], ip[MAX_IP_LEN], port[PORT_SIZE], msgResponse = RESPONSETYPE_CREATE_GROUP_SUCCES;
    char bufferSend[MAX_BUFFER_LENGTH];
    ConnectionData data;
    GroupData *groupData;
    ClientAppERR status;

    AskForGroupName(bufferGroupName);

    while (strlen(bufferGroupName) >= MAX_GROUP_NAME_SIZE)
    {
        PrintInvalidLength();
        AskForGroupName(bufferGroupName);
    }

    system("clear");

    if (AddGroupToList(_clientApp, bufferGroupName, _manu, &groupData) != CLIENT_APP_SUCCESS)
    {
        return CLIENT_APP_CONNECTION_FAILED;
    }

    _clientApp->bufferSize = PackCreateGroup(bufferSend, _clientApp->m_userName, bufferGroupName);

    if ((status = SendRecvToNetServer(_clientApp, bufferSend)) != CLIENT_APP_SUCCESS)
    {
        PrintFailServer();
        return status;
    }

    UnPackResponseCreateGroup(_clientApp->m_bufferReceive, messageLen, ip, port);

    if (strcmp(ip, "0") == EQUAL) /* in case that the create group failed - the ip is equal to zero */
    {
        msgResponse = RESPONSETYPE_CREATE_GROUP_FAILED;
        RemoveGroupFromList(_clientApp);
    }
    else
    {
        strcpy(data.ip, ip);
        strcpy(data.port, port);
        data.m_groupData = groupData;
    }

    return HandleFuncsControl(msgResponse, _manu, _clientApp, &data);
}

static ClientAppERR RenderGroupsToScreen(int *_manu, ClientApp *_clientApp)
{
    size_t messageLen, numOfGroups;
    Vector *myVec;
    char bufferSend[MAX_BUFFER_LENGTH], msgResponse = RESPONSETYPE_JOIN_RENDER_SUCCESS;
    ClientAppERR status;

    _clientApp->bufferSize = PackRender(bufferSend);

    if ((status = SendRecvToNetServer(_clientApp, bufferSend)) != CLIENT_APP_SUCCESS)
    {
        PrintFailServer();
        return status;
    }

    numOfGroups = RetrunNumOfGroups(_clientApp->m_bufferReceive);

    myVec = VectorCreate(numOfGroups, VECTOR_BLOCK_SIZE);
    if (myVec == NULL)
    {
        return CLIENT_APP_RENDER_FAIL;
    }

    if (UnPackRenderResponse(_clientApp->m_bufferReceive, messageLen, myVec) != TAG_ERR_SUCCESS)
    {
        /* tbd if malloc fail another response.*/
        msgResponse = RESPONSETYPE_JOIN_RENDER_FAILED;
    }

    if (!VectorSize(myVec))
    {
        msgResponse = RESPONSETYPE_JOIN_RENDER_FAILED;
    }
    else
    {
        msgResponse = RESPONSETYPE_JOIN_RENDER_SUCCESS;
    }

    return HandleFuncsControl(msgResponse, _manu, _clientApp, myVec);
}

static ClientAppERR ClientJoinGroup(int *_manu, ClientApp *_clientApp, Vector *_vector)
{
    ConnectionData data;
    char msgResponse, ip[MAX_IP_LEN], port[MAX_PORT_NUM], bufferSend[MAX_BUFFER_LENGTH];
    size_t messageLen, vecSize = VectorSize(_vector);
    void *pVal;
    char strInput[10];
    int option;
    GroupData *groupData;
    ClientAppERR status;

    GetUserOptionJoinGroup(strInput);

    while ((IsValidInput(strInput, &option, InputValidationJoinGroup, (void *)&vecSize)) == INVALID_INPUT)
    {
        PrintInvalidInput();
        GetUserOptionJoinGroup(strInput);
    }
    system("clear");
    VectorGet(_vector, ((size_t)option) - 1, &pVal);

    if (AddGroupToList(_clientApp, (char *)pVal, _manu, &groupData) != CLIENT_APP_SUCCESS)
    {
        return CLIENT_APP_CONNECTION_FAILED;
    }

    _clientApp->bufferSize = PackJoinGroup(bufferSend, _clientApp->m_userName,
                                           (char *)pVal);

    if ((status = SendRecvToNetServer(_clientApp, bufferSend)) != CLIENT_APP_SUCCESS)
    {
        PrintFailServer();
        return status;
    }

    UnPackResponseJoinGroup(_clientApp->m_bufferReceive, messageLen, ip, port);

    if (strcmp(ip, "0") == EQUAL) /* in case that the create group failed - the ip is equal to zero */
    {
        msgResponse = RESPONSETYPE_JOIN_GROUP_FAIL;
        RemoveGroupFromList(_clientApp);
    }

    else
    {
        msgResponse = RESPONSETYPE_JOIN_GROUP_SUCCESS;
    }

    strcpy(data.ip, ip);
    strcpy(data.port, port);
    data.m_groupData = groupData;

    return HandleFuncsControl(msgResponse, _manu, _clientApp, &data);
}

static ClientAppERR LeaveGroup(int *_manu, ClientApp *_clientApp)
{
    char userInputStr[4], bufferSend[MAX_BUFFER_LENGTH], msgResponse;
    int userInputInt;
    ListItr begin = ListItrBegin(_clientApp->m_listGroupNames), end = ListItrEnd(_clientApp->m_listGroupNames);
    size_t groupNum = 0, i = 1, messageLen;
    GroupData *groupReomved;
    ClientAppERR status;

    if (begin == end)
    {
        PrintNoGroups();
        return CLIENT_APP_LEAVE_FAIL;
    }

    PrintLeaveGroup();

    ListItrForEach(begin, end, PrintListAction, &groupNum);

    GetLeaveGroup(userInputStr);

    while ((IsValidInput(userInputStr, &userInputInt,
                         InputValidationLeaveGroup, (void *)&(groupNum))) == INVALID_INPUT)
    {
        PrintInvalidInput();
        GetUserOptionJoinGroup(userInputStr);
    }
    system("clear");
    while (i < userInputInt)
    {
        begin = ListItrNext(begin);
        i++;
    }

    groupReomved = (GroupData *)ListItrRemove(begin);

    _clientApp->bufferSize = PackLeaveGroup(bufferSend, _clientApp->m_userName,
                                            groupReomved->m_groupName);

    if ((status = SendRecvToNetServer(_clientApp, bufferSend)) != CLIENT_APP_SUCCESS)
    {
        PrintFailServer();
        return status;
    }

    UnPackResponseLeaveGroup(_clientApp->m_bufferReceive, messageLen, &msgResponse);

    return HandleFuncsControl(msgResponse, _manu, _clientApp, groupReomved);
}

static ClientAppERR LogOut(int *_manu, ClientApp *_clientApp)
{
    char bufferSend[MAX_BUFFER_LENGTH], msgResponse;
    size_t messageLen;
    ClientAppERR status;

    _clientApp->bufferSize = PackLogout(bufferSend, _clientApp->m_userName);

    if ((status = SendRecvToNetServer(_clientApp, bufferSend)) != CLIENT_APP_SUCCESS)
    {
        PrintFailServer();
        return status;
    }

    UnPackResponseLogout(_clientApp->m_bufferReceive, messageLen, &msgResponse);

    return HandleFuncsControl(msgResponse, _manu, _clientApp, NULL);
}

/* ----------------------------- General Functions ----------------------------- */

static ClientAppERR AddGroupToList(ClientApp *_clientApp, char *_groupName, int *_manu, GroupData **_groupData)
{
    int groupNameSize = strlen(_groupName);
    GroupData *groupData;

    if ((groupData = (GroupData *)malloc(sizeof(GroupData))) == NULL)
    {
        HandleCreateGroupFail(_manu, _clientApp, NULL);
        return CLIENT_APP_CONNECTION_FAILED;
    }

    strcpy(groupData->m_groupName, _groupName);
    groupData->m_groupName[groupNameSize] = '\0';

    if (ListPushHead(_clientApp->m_listGroupNames, groupData) != LIST_SUCCESS)
    {
        free(groupData);
        HandleCreateGroupFail(_manu, _clientApp, NULL);
        return CLIENT_APP_CONNECTION_FAILED;
    }

    *_groupData = groupData;

    return CLIENT_APP_SUCCESS;
}

static void RemoveGroupFromList(ClientApp *_clientApp)
{
    void *itemRemoved;

    ListPopHead(_clientApp->m_listGroupNames, &itemRemoved);

    free((GroupData *)itemRemoved);
}

static ClientAppERR SendRecvToNetServer(ClientApp *_clientApp, char *_buffSend)
{
    if ((ClientSend(_clientApp->m_socket, _buffSend, _clientApp->bufferSize)) !=
        CLIENT_NET_SUCCESS)
    {
        return CLIENT_APP_SEND_FAILED;
    }

    if ((ClientRecv(_clientApp->m_socket, _clientApp->m_bufferReceive)) != CLIENT_NET_SUCCESS)
    {
        return CLIENT_APP_RECV_FAILED;
    }
    return CLIENT_APP_SUCCESS;
}

/* ----------------------------- Handle Functions ----------------------------- */

static ClientAppERR HandleFuncsControl(char _msg, int *_mane, ClientApp *_clientApp, void *_context)
{
    HandleFunc arr[RESPONSETYPE_NUM_OF_RESPONSES] =
        {&HandleRegFail, &HandleRegSuccess,
         &HandleLoginFail, &HandleLoginSuccess,
         &HandleCreateGroupFail, &HandleCreateGroupSuccess,
         &HandleRenderGroupFail, &HandleRenderGroupSuccess,
         &handleJoinGroupFail, &HandleJoinGroupSuccess,
         &HandleLeaveGroup, &HandleLeaveGroup,
         &HandleLogoutSuccess, &HandleLogoutSuccess};

    arr[(int)_msg](_mane, _clientApp, _context);
    return CLIENT_APP_SUCCESS;
}

static void HandleRegSuccess(int *_mane, ClientApp *_clientApp, void *_context)
{
    *_mane = MAIN_SCREEN;
    PrintResponseRegiSucc();
}

static void HandleRegFail(int *_mane, ClientApp *_clientApp, void *_context)
{
    *_mane = MAIN_SCREEN;
    PrintResponseRegiFail();
}

static void HandleLoginSuccess(int *_mane, ClientApp *_clientApp, void *_context)
{
    *_mane = LOGGED_SCREEN;
    PrintResponseLoginSucc();
}

static void HandleLoginFail(int *_mane, ClientApp *_clientApp, void *_context)
{
    *_mane = MAIN_SCREEN;
    PrintResponseLoginFail();
}

static void HandleCreateGroupSuccess(int *_mane, ClientApp *_clientApp, void *_context)
{
    PrintCreateGroupSuccess();
    *_mane = LOGGED_SCREEN;
    OpenGroupChatWindows(((ConnectionData *)_context)->ip, ((ConnectionData *)_context)->port,
                         ((ConnectionData *)_context)->m_groupData->m_groupName, _clientApp->m_userName);
    GetPIDsToKill(&(((ConnectionData *)_context)->m_groupData->m_pidListener),
                  &(((ConnectionData *)_context)->m_groupData->m_pidSender));
}

static void HandleCreateGroupFail(int *_mane, ClientApp *_clientApp, void *_context)
{
    *_mane = LOGGED_SCREEN;
    PrintCreateGroupFail();
}

static void HandleRenderGroupFail(int *_mane, ClientApp *_clientApp, void *_context)
{
    PrintEmptyGroup();
    *_mane = LOGGED_SCREEN;
    VectorDestroy(((Vector **)&_context), DestroyVector);
}

static void HandleRenderGroupSuccess(int *_mane, ClientApp *_clientApp, void *_context)
{
    size_t counter = 0;
    PrintOptionManu();
    VectorForEach((Vector *)_context, RenderGroupsToForEach, &counter);
    ClientJoinGroup(_mane, _clientApp, (Vector *)_context);
}

static void handleJoinGroupFail(int *_mane, ClientApp *_clientApp, void *_context)
{
    printErrorJoinGroup();
    *_mane = LOGGED_SCREEN;
}

static void HandleJoinGroupSuccess(int *_mane, ClientApp *_clientApp, void *_context)
{
    *_mane = LOGGED_SCREEN;
    OpenGroupChatWindows(((ConnectionData *)_context)->ip, ((ConnectionData *)_context)->port,
                         ((ConnectionData *)_context)->m_groupData->m_groupName, _clientApp->m_userName);
    GetPIDsToKill(&(((ConnectionData *)_context)->m_groupData->m_pidListener),
                  &(((ConnectionData *)_context)->m_groupData->m_pidSender));
}

static void HandleLeaveGroup(int *_mane, ClientApp *_clientApp, void *_context)
{
    *_mane = LOGGED_SCREEN;
    PrintLeaveSucess(((GroupData *)_context)->m_groupName);
    KillListenSenderProcess(((GroupData *)_context)->m_pidListener,
                            ((GroupData *)_context)->m_pidSender);
    free(((GroupData *)_context));
}

static void HandleLogoutSuccess(int *_mane, ClientApp *_clientApp, void *_context)
{
    ListItr begin = ListItrBegin(_clientApp->m_listGroupNames), end = ListItrEnd(_clientApp->m_listGroupNames);
    GroupData *tempGroupData;
    *_mane = MAIN_SCREEN;

    while (begin != end)
    {
        tempGroupData = (GroupData *)ListItrGet(begin);
        KillListenSenderProcess(tempGroupData->m_pidListener,
                                tempGroupData->m_pidSender);
        begin = ListItrNext(begin);
        tempGroupData = (GroupData *)ListItrRemove(ListItrPrev(begin));
        free(tempGroupData);
    }

    PrintLogoutSuccess();
}

/* ----------------------------- Input Functions ----------------------------- */

static void GetUsername(ClientApp *_clientApp)
{
    int result;

    AskForUserName(_clientApp->m_userName);

    while (strlen(_clientApp->m_userName) >= MAX_NAME_LENGTH)
    {
        PrintInvalidLength();
        AskForUserName(_clientApp->m_userName);
    }
}

static void GetUserPassword(ClientApp *_clientApp)
{
    int result;

    AskForPassword(_clientApp->m_password);
    while (strlen(_clientApp->m_password) >= MAX_NAME_LENGTH)
    {
        PrintInvalidLength();
        AskForPassword(_clientApp->m_password);
    }
}

int IsValidInput(char *_userInputString, int *_userInputInt, int (*InputValidationFunc)(int _input, void *_context), void *_context)
{
    int i = 0;

    while (_userInputString[i] != '\0')
    {
        if (isdigit(_userInputString[i]) == 0)
        {
            return INVALID_INPUT;
        }
        i++;
    }

    *_userInputInt = atoi(_userInputString);

    return InputValidationFunc(*_userInputInt, _context);
}

static int InputValidationMainScreen(int _input, void *_context)
{
    if (_input != REGISTRATION && _input != LOGIN && _input != EXIT)
    {
        return INVALID_INPUT;
    }
    VALID_INPUT;
}

static int InputValidationLoginScreen(int _input, void *_context)
{
    if (_input != CREATE_GROUP && _input != CREATE_GROUP && _input != JOIN_GROUP &&
        _input != LEAVE_GROUP && _input != LOG_OUT)
    {
        return INVALID_INPUT;
    }
    VALID_INPUT;
}

static int InputValidationJoinGroup(int _input, void *_context)
{
    if (_input > *(int *)_context || _input <= 0)
    {
        return INVALID_INPUT;
    }
    VALID_INPUT;
}

static int InputValidationLeaveGroup(int _input, void *_context)
{
    if (_input > *(int *)_context || _input <= 0)
    {
        return INVALID_INPUT;
    }
    VALID_INPUT;
}

/* ----------------------------- General Functions ----------------------------- */

static void ListGroupDestroyItem(void *_element)
{
    free((GroupData *)_element);
}

static void DestroyVector(void *_item)
{
    free((char *)_item);
}

static int RenderGroupsToForEach(void *_element, size_t _index, void *_context)
{
    size_t optionVal;
    optionVal = ++*(size_t *)_context;
    PrintIndexGroup(optionVal, (char *)_element);
    return 1;
}

static int PrintListAction(void *_element, void *_context)
{
    (*(size_t *)_context)++;
    PrintIndexGroup((*(size_t *)_context), ((GroupData *)_element)->m_groupName);
    return 1;
}
