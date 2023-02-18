#include <stdio.h>  /* files */
#include <stdlib.h> /* malloc */
#include <string.h> /* strcmp */

#include "user_manager.h"
#include "hash_map.h"
#include "user.h"

#define NULL_CHARACTER ('\0')
#define RES_BUFF_LIMIT (2)
#define SPACE (" ")
#define HASH_SIZE (100)
#define FILE_NAME ("../server_main/users.txt")
#define FILE_MODE ("r")
#define FILE_MODE_WRITE ("a")
#define MAX_BUFF_SIZE (4095)

typedef enum
{
    FILE_OPEN_ERR = 1,
    USER_CREATE_FAIL,
    ERR_ALLOCATION_FAIL,
    INSERT_FAIL,
    ERR_WRITE_FILE_FAIL,
    FILE_SUCCESS,
    PARSER_FAIL,
    PARSER_SUCCESS

} internalErr;

struct UserManager
{
    HashMap *m_hash;
};

static size_t HashFunc(void *_key);
static int EqualityFunc(void *_firstData, void *_secondData);
static size_t HashFunc(void *_key);
static size_t calcAsciiVal(char *_elem);
static int Parser(char *buff, char **res);
static internalErr LoadData(UserManager *UserManager);
static internalErr saveInFile(char *name, char *_password);
static void KeyDestroy(void *_key);
static void ValDestroy(void *_value);

/* debug */
#if DEBUG
int isUserLooged(User *user);
int forEachAction(const void *_key, void *_value, void *_context);
void PrintUserList(User *user);
void TestPrintUserList(UserManager *_userManager, char *username);
#endif

UserManager *CreateUserMan()
{
    UserManager *user;
    user = (UserManager *)malloc(sizeof(UserManager));
    if (user == NULL)
    {
        return NULL;
    }

    user->m_hash = HashMapCreate(HASH_SIZE, HashFunc, EqualityFunc);
    if (user->m_hash == NULL)
    {
        free(user);
    }

    if (LoadData(user) != FILE_SUCCESS)
    {
        DestroyUserMan(&user);
        return NULL;
    }

#if DEBUG

    HashMapForEach(user->m_hash, forEachAction, NULL);
#endif

    return user;
}

statusUserManager AddNewUser(UserManager *_UserManager, char *_name, char *_password)
{
    char *key;
    User *user;
    void *pVal;
    size_t len;

    if (_UserManager == NULL)
    {
        return ERR_MANAGER_UNINITIALIZED;
    }

    if (_name == NULL || _password == NULL)
    {
        return INVALID_NAME_OR_PASS;
    }

    if (MAP_SUCCESS == HashMapFind(_UserManager->m_hash, _name, &pVal))
    {
        return ERR_USER_EXIST;
    }

    if ((user = CreateUser(_name, _password)) == NULL)
    {
        return ERR_ADD_USER_FAIL;
    }

    len = strlen(_name);
    key = (char *)malloc(len + 1);
    if (key == NULL)
    {
        DestroyUser(user);
        return INSERT_FAIL;
    }

    strcpy(key, _name);

    key[len] = '\0';

    if (HashMapInsert(_UserManager->m_hash, key, user) != MAP_SUCCESS)
    {
        DestroyUser(user);
        return INSERT_FAIL;
    }

    if (saveInFile(_name, _password) != FILE_SUCCESS)
    {
        DestroyUser(user);
        return USER_MANAGER_FILE_FAIL;
    }

    return SUCCESS_USER_MANAGER;
}

void DestroyUserMan(UserManager **_userManager)
{
    if (_userManager != NULL && *_userManager)
    {
        HashMapDestroy(&((*_userManager)->m_hash), KeyDestroy, ValDestroy);
        free(*_userManager);
    }
}

statusUserManager LoginUser(UserManager *_userManager, char *_name, char *_password)
{
    void *pValue;
    if (_userManager == NULL || _password == NULL)
    {
        return ERR_MANAGER_UNINITIALIZED;
    }

    if (HashMapFind(_userManager->m_hash, (char *)_name, &pValue) != MAP_SUCCESS)
    {
        return USER_NOT_EXIST;
    }

    if (strcmp(GetUserName((User *)pValue), _name) ||
        strncmp(GetPassword((User *)pValue), _password, strlen(_password)))
    {
        return USER_NOT_EXIST;
    }

    if (IsLogged((User *)pValue))
    {
        return ERR_USER_LOGGED;
    }

    SetToLog((User *)pValue);

    return SUCCESS_USER_MANAGER;
}

statusUserManager LogoutUser(UserManager *_userManager, char *_userName)
{
    void *pValue;
    if (_userManager == NULL || _userName == NULL)
    {
        return ERR_MANAGER_UNINITIALIZED;
    }
    if (HashMapFind(_userManager->m_hash, (char *)_userName, &pValue) != MAP_SUCCESS)
    {
        return USER_NOT_EXIST;
    }

    SetToOffline((User *)pValue);

#if DEBUG

    printf("groups:(have to be empty.)\n");
    TestPrintUserList(_userManager, _userName);
#endif

    return SUCCESS_USER_MANAGER;
}

statusUserManager InserGroupToUser(UserManager *_userManager, char *_username, char *_groupName)
{
    UserERR errCode;
    void *user;
    if (_userManager == NULL)
    {
        return ERR_MANAGER_UNINITIALIZED;
    }
    if (_username == NULL)
    {
        return USER_NOT_EXIST;
    }
    if (_groupName == NULL)
    {
        return ERR_MANAGER_GROUP_UNINITIALIZED;
    }

    if (HashMapFind(_userManager->m_hash, _username, &user) != MAP_SUCCESS)
    {
        return USER_NOT_EXIST;
    }

    if (errCode = UpdateUserGroupList((User *)user, _groupName) != USER_SUCCESS)
    {
        return errCode;
    }

#if DEBUG

    printf("groups:\n");
    TestPrintUserList(_userManager, _username);
#endif

    return SUCCESS_USER_MANAGER;
}

statusUserManager LeaveGroupUserManagment(UserManager *_userManager, char *_userName,
                                          char *_groupName)
{
    UserERR errCode;
    void *user;

#if DEBUG

    printf("user groups before leave:\n");
    TestPrintUserList(_userManager, _userName);

#endif

    if (_userManager == NULL)
    {
        return ERR_MANAGER_UNINITIALIZED;
    }

    if (_userName == NULL)
    {
        return USER_NOT_EXIST;
    }

    if (_groupName == NULL)
    {
        return GROUP_NOT_EXIST;
    }

    if (HashMapFind(_userManager->m_hash, _userName, &user) != MAP_SUCCESS)
    {
        return USER_NOT_EXIST;
    }

    if (errCode = UserLeaveGroup((User *)user, _groupName) != USER_SUCCESS)
    {
        return errCode;
    }

#if DEBUG

    printf("user groups after leave:\n");
    TestPrintUserList(_userManager, _userName);

#endif

    return SUCCESS_USER_MANAGER;
}

int IsUserLoggedUserManager(UserManager *_userManager, char *_userName)
{
    void *pValue;
    if (_userManager == NULL || _userName == NULL)
    {
        return FALSE;
    }

    if (HashMapFind(_userManager->m_hash, _userName, &pValue) != MAP_SUCCESS)
    {
        return FALSE;
    }

    return IsLogged((User *)pValue);
}

size_t GetNumOfUserGroupsUserManager(UserManager *_userManager, char *_userName)
{
    void *pValue;
    if (_userManager == NULL || _userName == NULL)
    {
        return 0;
    }

    if (HashMapFind(_userManager->m_hash, _userName, &pValue) != MAP_SUCCESS)
    {
        return 0;
    }

    return userGetNumOfGroups((User *)pValue);
}

statusUserManager RemoveUserGroup(UserManager *_userManager, char *_userName, char *_buff)
{
    UserERR errCode;
    void *pVal;
    if (_userManager == NULL || _userName == NULL)
    {
        return ERR_MANAGER_UNINITIALIZED;
    }

    if (HashMapFind(_userManager->m_hash, _userName, &pVal) != MAP_SUCCESS)
    {
        return USER_NOT_EXIST;
    }

    if ((errCode = RemoveUserFirstGroup((User *)pVal, _buff)) != USER_SUCCESS)
    {
        return errCode;
    }

    return SUCCESS_USER_MANAGER;
}

int HasJoinedManager(UserManager *_userManager, char *m_userName, char *_groupName)
{
    void *pVal;
    if (_userManager == NULL || _groupName == NULL)
    {
        return TRUE;
    }

    if (HashMapFind(_userManager->m_hash, m_userName, &pVal) != MAP_SUCCESS)
    {
        return TRUE;
    }

    return HasJoined((User *)pVal, _groupName);
}

static void ValDestroy(void *_value)
{
    DestroyUser((User *)_value);
}

static void KeyDestroy(void *_key)
{
    free((char *)_key);
}

static internalErr saveInFile(char *_name, char *_password)
{
    FILE *fp;
    size_t nameLen;
    char endOfSen = '\n';
    char newFiled[MAX_BUFF_SIZE];

    if ((fp = fopen(FILE_NAME, FILE_MODE_WRITE)) == NULL)
    {
        return FILE_OPEN_ERR;
    }

    strcpy(newFiled, _name);
    nameLen = strlen(newFiled);
    newFiled[nameLen] = ' ';
    strcpy(((char *)newFiled) + 1 + nameLen, _password);

    if (fprintf(fp, "%s", newFiled) < 0 || fputc(endOfSen, fp) < 0)
    {
        return ERR_WRITE_FILE_FAIL;
    }

    fclose(fp);

    return FILE_SUCCESS;
}

static internalErr LoadData(UserManager *_userManager)
{
    User *user;
    FILE *fp;
    char buff[MAX_BUFF_SIZE] = {0};
    char *auxStorage[RES_BUFF_LIMIT];

    if ((fp = fopen(FILE_NAME, FILE_MODE)) == NULL)
    {
        return FILE_OPEN_ERR;
    }

    while (fgets(buff, MAX_BUFF_SIZE, fp) != NULL)
    {
        char *key;
        size_t len = 0;

        if (PARSER_SUCCESS == Parser(buff, auxStorage))
        {
            if ((user = CreateUser(auxStorage[0], auxStorage[1])) == NULL)
            {
                return USER_CREATE_FAIL;
            }

            len = strlen(auxStorage[0]);

            if ((key = (char *)malloc(len + 1)) == NULL)
            {
                return ERR_ALLOCATION_FAIL;
            }

            strcpy(key, auxStorage[0]);

            key[len] = NULL_CHARACTER;

            if (HashMapInsert(_userManager->m_hash, key, user) != MAP_SUCCESS)
            {
                return INSERT_FAIL;
            }
        }
    }

    fclose(fp);

    return FILE_SUCCESS;
}

static int EqualityFunc(void *_firstData, void *_secondData)
{
    return strcmp(GetUserName((User *)_firstData), GetUserName((User *)_secondData));
}

static size_t HashFunc(void *_key)
{
    return calcAsciiVal((char *)_key) * 2;
}

static size_t calcAsciiVal(char *_elem)
{
    size_t val = 0;

    for (; *_elem != NULL_CHARACTER; ++_elem)
    {
        val += (int)*_elem;
    }

    return val;
}

static int Parser(char *buff, char **res)
{
    const char s[RES_BUFF_LIMIT] = " ";
    char *token;

    while (*buff == ' ')
    {
        ++buff;
    }

    if (*buff == '\0' || *buff == '\n' || *buff == ' ')
    {
        return PARSER_FAIL;
    }

    token = strtok(buff, s);
    res[0] = token;

    token = strtok(NULL, s);
    res[1] = token;

    return PARSER_SUCCESS;
}

/* debug */
#if DEBUG
int forEachAction(const void *_key, void *_value, void *_context)
{
    printf("key for each = %s\n", (char *)_key);
}

void TestPrintUserList(UserManager *_userManager, char *username)
{
    void *pval;
    HashMapFind(_userManager->m_hash, username, &pval);
    PrintUserList((User *)pval);
}
int isLogged(UserManager *_userManager)
{
    return isUserLooged(_userManager->m_hash);
}
#endif
