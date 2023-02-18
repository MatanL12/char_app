#include "group.h"
#include "limitations.h"
#include "data_net.h"

#include <stdlib.h> /* malloc */
#include <string.h> /* strcat */
#include <stddef.h> /* size_t */

#define MAGIC_NUMBER 942357

/* --------------------------------- Structs ----------------------------  */

struct Group
{
    char *m_name;
    char m_ip[MAX_IP_LEN];
    char m_port[PORT_SIZE];
    size_t m_userCounter;
    int m_magicNumber;
};

/* ---------------------- Static Function Declaration --------------------  */

/* ----------------------------------- API ------------------------------- */

Group *CreateGroup(char *_ip, char *_name, char *_port)
{
    Group *ptrGroup;

    if (_ip == NULL || _name == NULL || _port == NULL)
    {
        return NULL;
    }

    if ((ptrGroup = (Group *)malloc(sizeof(Group))) == NULL)
    {
        return NULL;
    }

    ptrGroup->m_name = _name;
    strcpy(ptrGroup->m_ip, _ip);
    strcpy(ptrGroup->m_port, _port);
    ptrGroup->m_userCounter = 0;
    ptrGroup->m_magicNumber = MAGIC_NUMBER;

    return ptrGroup;
}


void DestroyGroup(Group *_group)
{
    if (_group != NULL && _group->m_magicNumber == MAGIC_NUMBER)
    {
        _group->m_magicNumber = 0;
        free(_group);
    }
}


GroupErr IncreaseUsersCounter(Group *_group)
{
    if (_group == NULL)
    {
        return GROUP_NOT_INITIALIZED;
    }

    (_group->m_userCounter)++;

    return GROUP_SUCCESS;
}


GroupErr DecreaseUsersCounter(Group *_group)
{
    if (_group == NULL)
    {
        return GROUP_NOT_INITIALIZED;
    }

    (_group->m_userCounter)--;

    if (_group->m_userCounter == 0)
    {
        return GROUP_EMPTY;
    }

    return GROUP_SUCCESS;
}


GroupErr GetGroupIp(Group *_group, char *_ip)
{
    if (_group == NULL || _ip == NULL)
    {
        return GROUP_NOT_INITIALIZED;
    }

    strcpy(_ip, _group->m_ip);

    return GROUP_SUCCESS;
}


GroupErr GetGroupPort(Group *_group, char *_port)
{
    if (_group == NULL || _port == NULL)
    {
        return GROUP_NOT_INITIALIZED;
    }

    strcpy(_port, _group->m_port);

    return GROUP_SUCCESS;
}


GroupErr GetGroupName(Group *_group, char **_name)
{
    if (_group == NULL || _name == NULL)
    {
        return GROUP_NOT_INITIALIZED;
    }

    *_name = _group->m_name;

    return GROUP_SUCCESS;
}
