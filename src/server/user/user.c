#include <stdlib.h> /* malloc */
#include <string.h> /* strcpy */
#include <stdio.h>  /* for debug */

#include "user.h"
#include "limitations.h"
#include "g_d_linked_list.h"
#include "d_linked_list_function.h"
#include "list_itr.h"

#define OFFLINE 0
#define ONLINE 1

#define FALSE (0)

#define MAGIC_NUMBER 941234

struct User
{
    char m_userName[MAX_NAME_LENGTH];
    char m_password[MAX_PASSWORD_LENGTH];
    int m_isConnect;
    List *m_groupsList;
    int m_magicNumber;
};

static int PredicateFindInList(void *_element, void *_context);

/* debug */
#if DEBUG
static int forEachPrintGroup(void *_elem, void *_context);
#endif

User *CreateUser(char *_userName, char *_password)
{
    User *ptrUser;

    if (_userName == NULL || _password == NULL)
    {
        return NULL;
    }

    if ((ptrUser = (User *)malloc(sizeof(User))) == NULL)
    {
        return NULL;
    }

    if ((ptrUser->m_groupsList = ListCreate()) == NULL)
    {
        free(ptrUser);
        return NULL;
    }

    strcpy(ptrUser->m_userName, _userName);
    strcpy(ptrUser->m_password, _password);

    ptrUser->m_isConnect = OFFLINE;
    ptrUser->m_magicNumber = MAGIC_NUMBER;

    return ptrUser;
}

void DestroyUser(User *_user)
{
    if (_user != NULL && _user->m_magicNumber == MAGIC_NUMBER)
    {
        ListDestroy(&(_user->m_groupsList), NULL);
        _user->m_magicNumber = 0;
        free(_user);
    }
}

char *GetUserName(User *_user)
{
    if (_user == NULL)
    {
        return NULL;
    }

    return _user->m_userName;
}

char *GetPassword(User *_user)
{
    if (_user == NULL)
    {
        return NULL;
    }

    return _user->m_password;
}

void SetToLog(User *_user)
{
    if (_user != NULL)
    {
        _user->m_isConnect = ONLINE;
    }
}

UserERR UpdateUserGroupList(User *_user, char *_groupName)
{
    if (_user == NULL || _groupName == NULL)
    {
        return USER_NOT_INITIALIZED;
    }

    if (ListPushTail(_user->m_groupsList, _groupName) != LIST_SUCCESS)
    {
        return ADD_GROUP_FAIL;
    }

    return USER_SUCCESS;
}

UserERR UserLeaveGroup(User *_user, char *_groupName)
{
    ListItr begin = ListItrBegin(_user->m_groupsList);
    ListItr end = ListItrEnd(_user->m_groupsList);
    ListItr result;
    if (_user == NULL || _groupName == NULL)
    {
        return USER_NOT_INITIALIZED;
    }

    result = ListItrFindFirst(begin, end, PredicateFindInList, _groupName);

    if (ListItrEquals(result, end))
    {
        return USER_GROUP_NOT_EXIST;
    }

    ListItrRemove(result);

    return USER_SUCCESS;
}

UserERR RemoveUserFirstGroup(User *_user, char *buff)
{
    void *pVal;
    if (_user == NULL || buff == NULL)
    {
        return USER_NOT_INITIALIZED;
    }

    ListPopHead(_user->m_groupsList, &pVal);

    strcpy(buff, (char *)pVal);

    return USER_SUCCESS;
}

int IsLogged(User *_user)
{
    if (_user == NULL)
    {
        return FALSE;
    }

    return _user->m_isConnect == ONLINE;
}

void SetToOffline(User *_user)
{
    if (_user != NULL)
    {
        _user->m_isConnect = OFFLINE;
    }
}

size_t userGetNumOfGroups(User *_user)
{
    if (_user == NULL)
    {
        return 0;
    }

    return ListSize(_user->m_groupsList);
}

int HasJoined(User *_user, char *_groupName)
{
    ListItr begin;
    ListItr end;

    if (_user == NULL)
    {
        return 1;
    }

    begin = ListItrBegin(_user->m_groupsList);
    end = ListItrEnd(_user->m_groupsList);

    return !ListItrEquals(ListItrFindFirst(begin, end, PredicateFindInList, _groupName), end);
}

static int PredicateFindInList(void *_element, void *_context)
{
    if (strcmp((char *)_element, (char *)_context) == 0)
    {
        return 0;
    }

    return 1;
}

/* debug */
#if DEBUG
void PrintUserList(User *user)
{
    ListItr begin = ListItrBegin(user->m_groupsList);
    ListItr end = ListItrEnd(user->m_groupsList);
    ListItrForEach(begin, end, forEachPrintGroup, NULL);
}

static int forEachPrintGroup(void *_elem, void *_context)
{
    printf("%s\n", (char *)_elem);
}
#endif
