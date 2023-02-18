#include "protocol.h"
#include "limitations.h"
#include <string.h> /*  strlen, strncpy */
#include <stdio.h>  /* remove */

#define PROTOCOL_TAG_INDEX (0)
#define PROTOCOL_TOTAL_LEN_INDEX (1)

/************************************** API FUNCTIONS ***************************************/

Tags RetrunTag(char _buffer[])
{
    if (_buffer == NULL)
    {
        return TAG_FAIL;
    }

    return _buffer[PROTOCOL_TAG_INDEX];
}

size_t PackRegistration(char _buffer[], char _userName[], char _pass[])
{
    size_t counter = 0;
    int lenUsrName, lenPass;

    if (_buffer == NULL || _userName == NULL || _pass == NULL)
    {
        return 0;
    }

    lenUsrName = strlen(_userName);
    lenPass = strlen(_pass);

    _buffer[counter++] = TAG_REGISTER;
    _buffer[counter++] = lenUsrName + lenPass + 2;
    _buffer[counter++] = lenUsrName;

    while (*_userName != '\0')
    {
        _buffer[counter++] = *_userName;
        _userName++;
    }

    _buffer[counter++] = lenPass;

    while (*_pass != '\0')
    {
        _buffer[counter++] = *_pass;
        _pass++;
    }

    return counter;
}

TagStatus UnPackRegistration(char _buffer[], size_t _messageLen, char _userName[], char _pass[])
{
    size_t passLenIndex;

    if (_buffer == NULL || _userName == NULL || _pass == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_REGISTER)
    {
        return TAG_ERR_MISMATCH;
    }

    /* if (_messageLen != _buffer[PROTOCOL_TOTAL_LEN_INDEX] || _messageLen <= 2)
     {
         return TAG_ERR_MESSAGE_NOT_FULL;
     } */

    strncpy(_userName, &_buffer[3], (size_t)_buffer[2]);
    _userName[(size_t)_buffer[2]] = '\0';

    passLenIndex = 3 + (size_t)_buffer[2];

    strncpy(_pass, &_buffer[passLenIndex + 1], (size_t)_buffer[passLenIndex]);
    _pass[_buffer[passLenIndex]] = '\0';

    return TAG_ERR_SUCCESS;
}

size_t PackLogin(char _buffer[], char _userName[], char _pass[])
{
    TagStatus answer = PackRegistration(_buffer, _userName, _pass);

    if (_buffer != NULL)
    {
        _buffer[PROTOCOL_TAG_INDEX] = TAG_LOGIN;
    }

    return answer;
}

size_t RetrunNumOfGroups(char _buffer[])
{
    if (_buffer == NULL)
    {
        return 0;
    }

    return _buffer[3];
}

TagStatus UnPackLogin(char _buffer[], size_t _messageLen, char _userName[], char _pass[])
{
    size_t passLenIndex;

    if (_buffer == NULL || _userName == NULL || _pass == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_LOGIN)
    {
        return TAG_ERR_MISMATCH;
    }

    /* if (_messageLen != _buffer[PROTOCOL_TOTAL_LEN_INDEX] || _messageLen <= 2)
    {
        return TAG_ERR_MESSAGE_NOT_FULL;
    } */

    strncpy(_userName, &_buffer[3], (size_t)_buffer[2]);
    _userName[(size_t)_buffer[2]] = '\0';

    passLenIndex = 3 + (size_t)_buffer[2];

    strncpy(_pass, &_buffer[passLenIndex + 1], (size_t)_buffer[passLenIndex]);
    _pass[_buffer[passLenIndex]] = '\0';

    return TAG_ERR_SUCCESS;
}


size_t PackResponseReg(char _buffer[], char _msg)
{
    size_t counter = 0;
    int lenMsg;

    if (_buffer == NULL)
    {
        return 0;
    }

    _buffer[counter++] = TAG_REGISTER;
    _buffer[counter++] = 2;
    _buffer[counter++] = 1;

    _buffer[counter] = '\0';

    counter++;
    strncat(_buffer, &_msg, 1);

    return counter;
}

TagStatus UnPackResponseReg(char _buffer[], size_t _messageLen, char *_msg)
{
    if (_buffer == NULL || _msg == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_REGISTER)
    {
        return TAG_ERR_MISMATCH;
    }

    /*  if (_buffer[PROTOCOL_TOTAL_LEN_INDEX] != _messageLen)
     {
         return TAG_ERR_MESSAGE_NOT_FULL;
     } */

    *_msg = _buffer[3];

    return TAG_ERR_SUCCESS;
}

size_t PackResponseLogin(char _buffer[], char _msg)
{
    size_t counter = 0;
    int lenMsg;

    if (_buffer == NULL)
    {
        return 0;
    }

    _buffer[counter++] = TAG_LOGIN;
    _buffer[counter++] = 2;
    _buffer[counter++] = 1;

    _buffer[counter] = '\0';

    counter++;
    strncat(_buffer, &_msg, 1);

    return counter;
}

TagStatus UnPackResponseLogin(char _buffer[], size_t _messageLen, char *_msg)
{
    if (_buffer == NULL || _msg == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_LOGIN)
    {
        return TAG_ERR_MISMATCH;
    }

    /*  if (_buffer[PROTOCOL_TOTAL_LEN_INDEX] != _messageLen)
     {
         return TAG_ERR_MESSAGE_NOT_FULL;
     } */

    *_msg = _buffer[3];

    return TAG_ERR_SUCCESS;
}

size_t PackCreateGroup(char _buffer[], char _userName[], char _groupName[])
{
    size_t counter = 0;
    int lenUsrName, lenGroupName;

    if (_buffer == NULL || _userName == NULL || _groupName == NULL)
    {
        return 0;
    }

    lenUsrName = strlen(_userName);
    lenGroupName = strlen(_groupName);

    _buffer[counter++] = TAG_CREATE_GROUP;
    _buffer[counter++] = lenUsrName + lenGroupName + 2;
    _buffer[counter++] = lenUsrName;

    while (*_userName != '\0')
    {
        _buffer[counter++] = *_userName;
        _userName++;
    }

    _buffer[counter++] = lenGroupName;

    while (*_groupName != '\0')
    {
        _buffer[counter++] = *_groupName;
        _groupName++;
    }

    _buffer[counter] = '\0';

    return counter;
}

TagStatus UnPackCreateGroup(char _buffer[], size_t _messageLen, char _userName[], char _groupName[])
{
    size_t groupLenIndex;

    if (_buffer == NULL || _userName == NULL || _groupName == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_CREATE_GROUP)
    {
        return TAG_ERR_MISMATCH;
    }

    /* if (_messageLen != _buffer[PROTOCOL_TOTAL_LEN_INDEX] || _messageLen <= 2)
     { /* char ch = 0; */

    strncpy(_userName, &_buffer[3], (size_t)_buffer[2]);
    _userName[(size_t)_buffer[2]] = '\0';

    groupLenIndex = 3 + (size_t)_buffer[2];

    strncpy(_groupName, &_buffer[groupLenIndex + 1], (size_t)_buffer[groupLenIndex]);
    _groupName[_buffer[groupLenIndex]] = '\0';

    return TAG_ERR_SUCCESS;
}

size_t PackResponseCreateGroup(char *_buffer, char _ip[], char _port[])
{
    size_t counter = 0;
    int lenIP, lenPort;

    if (_buffer == NULL || _ip == NULL || _port == NULL)
    {
        return 0;
    }

    lenIP = strlen(_ip);
    lenPort = strlen(_port);

    _buffer[counter++] = TAG_CREATE_GROUP;
    _buffer[counter++] = lenIP + lenPort + 2;
    _buffer[counter++] = lenIP;

    while (*_ip != '\0')
    {
        _buffer[counter++] = *_ip;
        _ip++;
    }

    _buffer[counter++] = lenPort;

    while (*_port != '\0')
    {
        _buffer[counter++] = *_port;
        _port++;
    }

    return counter;
}

TagStatus UnPackResponseCreateGroup(char _buffer[], size_t _messageLen, char _ip[], char _port[])
{
    size_t groupLenIndex;

    if (_buffer == NULL || _ip == NULL || _port == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_CREATE_GROUP)
    {
        return TAG_ERR_MISMATCH;
    }

    /* if (_messageLen != _buffer[PROTOCOL_TOTAL_LEN_INDEX] || _messageLen <= 2)
    {
        return TAG_ERR_MESSAGE_NOT_FULL;
    } */

    strncpy(_ip, &_buffer[3], (size_t)_buffer[2]);
    _ip[(size_t)_buffer[2]] = '\0';

    groupLenIndex = 3 + (size_t)_buffer[2];

    strncpy(_port, &_buffer[groupLenIndex + 1], (size_t)_buffer[groupLenIndex]);
    _port[(size_t)_buffer[groupLenIndex]] = '\0';

    return TAG_ERR_SUCCESS;
}


/* ------------------------------ Render Groups ------------------------------ */


size_t PackJoinGroupRender(char *_buffer)
{
    size_t counter = 0;

    if (_buffer == NULL)
    {
        return 0;
    }

    _buffer[counter++] = TAG_JOIN_GROUP_RENDER;
    _buffer[counter++] = 0;

    return counter;
}

TagStatus UnPackJoinGroupRender(char _buffer[], size_t _messegeLen)
{

    if (_buffer == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_JOIN_GROUP_RENDER)
    {
        return TAG_ERR_MISMATCH;
    }

    /* if (_messageLen != _buffer[PROTOCOL_TOTAL_LEN_INDEX] || _messageLen <= 2)
    {
        return TAG_ERR_MESSAGE_NOT_FULL;
    } */

    return TAG_ERR_SUCCESS;
}

size_t PackRenderResponse(char _buffer[], Vector *_groups)
{
    size_t counter = 0, numOfGroups;
    size_t totalLen = 0, j;
    size_t i;
    char *groupNameI;

    size_t tempGroupLen;

    numOfGroups = VectorSize(_groups);

    _buffer[counter++] = TAG_JOIN_GROUP_RENDER; /* TAG Type */
    _buffer[counter++] = 0;                   /* Total Length - will be filled later !!!*/
    _buffer[counter++] = 1;                   /* Coutner Length */
    _buffer[counter++] = numOfGroups;         /* Counter value */

    for (i = 0; i < numOfGroups; i++)
    {
        VectorGet(_groups, i, (void **)&groupNameI);

        tempGroupLen = strlen(groupNameI);

        _buffer[counter++] = tempGroupLen; /* Length groupI */

        for (j = 0; j < tempGroupLen; j++)
        {
            _buffer[counter++] = groupNameI[j];
        }

        totalLen += tempGroupLen + 1;
    }

    _buffer[1] = totalLen + 2;

    return counter;
}

TagStatus UnPackRenderResponse(char _buffer[], size_t _messageLen, Vector *_groups)
{
    size_t GroupNum, i, j = 4;
    char *arrayOfGroupNames[MAX_ACTIVE_GROUP_NUMBER] = {0};

    if (_buffer == NULL || _groups == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_JOIN_GROUP_RENDER)
    {
        return TAG_ERR_MISMATCH;
    }

    GroupNum = (size_t)_buffer[3];

    for (i = 0; i < GroupNum; i++)
    {
        arrayOfGroupNames[i] = (char *)malloc((size_t)_buffer[j] + 1);
        j++;

        strncpy(arrayOfGroupNames[i], &_buffer[j], (size_t)_buffer[j - 1]);

        arrayOfGroupNames[i][(size_t)_buffer[j - 1]] = '\0';

        VectorAppend(_groups, arrayOfGroupNames[i]);

        j += (size_t)_buffer[j - 1];
    }

    return TAG_ERR_SUCCESS;
}


size_t PackRender(char _buffer[])
{
    size_t counter = 0;

    if (_buffer == NULL)
    {
        return 0;
    }

    _buffer[counter++] = TAG_JOIN_GROUP_RENDER;

    _buffer[counter++] = 0;

    return counter;
}


/* --------------------------- Join Group --------------------------- */

size_t PackJoinGroup(char _buffer[], char _userName[], char _groupName[])
{
    size_t counter = 0;
    int lenUsrName,lenGroupName;
    

    if (_buffer == NULL || _userName == NULL || _groupName == NULL)
    {
        return 0;
    }

    lenUsrName = strlen(_userName);
    lenGroupName = strlen(_groupName);

    _buffer[counter++] = TAG_JOIN_GROUP;
    _buffer[counter++] = lenUsrName + lenGroupName + 2;
    _buffer[counter++] = lenUsrName;

    while (*_userName != '\0')
    {
        _buffer[counter++] = *_userName;
        _userName++;
    }

    _buffer[counter++] = lenGroupName;

    while (*_groupName != '\0')
    {
        _buffer[counter++] = *_groupName;
        _groupName++;
    }

    _buffer[counter] = '\0';

    return counter;
}


TagStatus UnPackJoinGroup(char _buffer[], size_t _messageLen, char _userName[], char _groupName[])
{
    size_t groupLenIndex;

    if(_buffer == NULL || _userName == NULL || _groupName == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_JOIN_GROUP)
    {
        return TAG_ERR_MISMATCH;
    }

   /* if (_messageLen != _buffer[PROTOCOL_TOTAL_LEN_INDEX] || _messageLen <= 2)
    { /* char ch = 0; */

    strncpy(_userName, &_buffer[3], (size_t)_buffer[2]);
    _userName[(size_t)_buffer[2]] = '\0';

    groupLenIndex = 3 + (size_t)_buffer[2];

    strncpy(_groupName, &_buffer[groupLenIndex + 1], (size_t)_buffer[groupLenIndex]);
    _groupName[_buffer[groupLenIndex]] = '\0';   

    return TAG_ERR_SUCCESS;
}


size_t PackResponseJoinGroup(char* _buffer, char _ip[], char _port[])
{ 
    size_t counter = 0;
    int lenIP,lenPort;

    if (_buffer == NULL || _ip == NULL || _port == NULL)
    {
        return 0;
    }

    lenIP = strlen(_ip);
    lenPort = strlen(_port);

    _buffer[counter++] = TAG_JOIN_GROUP;
    _buffer[counter++] = lenIP + lenPort + 2;
    _buffer[counter++] = lenIP;

    while (*_ip != '\0')
    {
        _buffer[counter++] = *_ip;
        _ip++;
    }

    _buffer[counter++] = lenPort;

    while (*_port != '\0')
    {
        _buffer[counter++] = *_port;
        _port++;
    }

    return counter;
} 


TagStatus UnPackResponseJoinGroup(char _buffer[], size_t _messageLen, char _ip[], char _port[])
{
    size_t groupLenIndex;

    if(_buffer == NULL || _ip == NULL || _port == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_JOIN_GROUP)
    {
        return TAG_ERR_MISMATCH;
    }

    /* if (_messageLen != _buffer[PROTOCOL_TOTAL_LEN_INDEX] || _messageLen <= 2)
    {
        return TAG_ERR_MESSAGE_NOT_FULL;
    } */

    strncpy(_ip, &_buffer[3], (size_t)_buffer[2]);
    _ip[(size_t)_buffer[2]] = '\0';

    groupLenIndex = 3 + (size_t)_buffer[2];

    strncpy(_port, &_buffer[groupLenIndex + 1], (size_t)_buffer[groupLenIndex]);
    _port[(size_t)_buffer[groupLenIndex]] = '\0';

    return TAG_ERR_SUCCESS;
}



/* --------------------------- Leave Group --------------------------- */


size_t PackLeaveGroup(char _buffer[], char _userName[], char _groupName[])
{
    size_t counter = 0;
    int lenUsrName,lenGroupName;
    

    if (_buffer == NULL || _userName == NULL || _groupName == NULL)
    {
        return 0;
    }

    lenUsrName = strlen(_userName);
    lenGroupName = strlen(_groupName);

    _buffer[counter++] = TAG_LEAVE_GROUP;
    _buffer[counter++] = lenUsrName + lenGroupName + 2;
    _buffer[counter++] = lenUsrName;

    while (*_userName != '\0')
    {
        _buffer[counter++] = *_userName;
        _userName++;
    }

    _buffer[counter++] = lenGroupName;

    while (*_groupName != '\0')
    {
        _buffer[counter++] = *_groupName;
        _groupName++;
    }

    _buffer[counter] = '\0';

    return counter;
}


TagStatus UnPackLeaveGroup(char _buffer[], size_t _messageLen, char _userName[], char _groupName[])
{
    size_t groupLenIndex;

    if(_buffer == NULL || _userName == NULL || _groupName == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_LEAVE_GROUP)
    {
        return TAG_ERR_MISMATCH;
    }

   /* if (_messageLen != _buffer[PROTOCOL_TOTAL_LEN_INDEX] || _messageLen <= 2)
    { /* char ch = 0; */

    strncpy(_userName, &_buffer[3], (size_t)_buffer[2]);
    _userName[(size_t)_buffer[2]] = '\0';

    groupLenIndex = 3 + (size_t)_buffer[2];

    strncpy(_groupName, &_buffer[groupLenIndex + 1], (size_t)_buffer[groupLenIndex]);
    _groupName[_buffer[groupLenIndex]] = '\0';   

    return TAG_ERR_SUCCESS;
}



size_t PackResponseLeaveGroup(char _buffer[], char _msg)
{
    size_t counter = 0;
    int lenMsg;

    if (_buffer == NULL)
    {
        return 0;
    }

    _buffer[counter++] = TAG_LEAVE_GROUP;
    _buffer[counter++] = 2;
    _buffer[counter++] = 1;

    _buffer[counter] = '\0';

    counter++;
    strncat(_buffer, &_msg, 1);

    return counter;
}

TagStatus UnPackResponseLeaveGroup(char _buffer[], size_t _messageLen, char *_msg)
{
    if (_buffer == NULL || _msg == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_LEAVE_GROUP)
    {
        return TAG_ERR_MISMATCH;
    }

    /*  if (_buffer[PROTOCOL_TOTAL_LEN_INDEX] != _messageLen)
     {
         return TAG_ERR_MESSAGE_NOT_FULL;
     } */

    *_msg = _buffer[3];

    return TAG_ERR_SUCCESS;
}






/********************** LOGOUT **********************/



size_t PackLogout(char _buffer[], char _userName[])
{
    size_t counter = 0;
    int lenUsrName,lenGroupName;
    
    if (_buffer == NULL || _userName == NULL)
    {
        return 0;
    }

    lenUsrName = strlen(_userName);
 
    _buffer[counter++] = TAG_LOGOUT;
    _buffer[counter++] = lenUsrName +  1;
    _buffer[counter++] = lenUsrName;

    while (*_userName != '\0')
    {
        _buffer[counter++] = *_userName;
        _userName++;
    }

    _buffer[counter] = '\0';

    return counter;
}


TagStatus UnPackLogout(char _buffer[], size_t _messageLen, char _userName[])
{
    if (_buffer == NULL || _userName == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_LOGOUT)
    {
        return TAG_ERR_MISMATCH;
    }

    /*  if (_buffer[PROTOCOL_TOTAL_LEN_INDEX] != _messageLen)
    {
        return TAG_ERR_MESSAGE_NOT_FULL;
    } */

    strncpy(_userName, &_buffer[3], (size_t)_buffer[2]);
    _userName[(size_t)_buffer[2]] = '\0';


    return TAG_ERR_SUCCESS;
}




size_t PackResponseLogout(char _buffer[], char _msg)
{
    size_t counter = 0;
    int lenMsg;

    if (_buffer == NULL)
    {
        return 0;
    }

    _buffer[counter++] = TAG_LOGOUT;
    _buffer[counter++] = 2;
    _buffer[counter++] = 1;

    _buffer[counter] = '\0';

    counter++;
    strncat(_buffer, &_msg, 1);

    return counter;
}

TagStatus UnPackResponseLogout(char _buffer[], size_t _messageLen, char *_msg)
{
    if (_buffer == NULL || _msg == NULL)
    {
        return TAG_ERR_NOT_INITIALIZED;
    }

    if (_buffer[PROTOCOL_TAG_INDEX] != TAG_LOGOUT)
    {
        return TAG_ERR_MISMATCH;
    }

    /*  if (_buffer[PROTOCOL_TOTAL_LEN_INDEX] != _messageLen)
     {
         return TAG_ERR_MESSAGE_NOT_FULL;
     } */

    *_msg = _buffer[3];

    return TAG_ERR_SUCCESS;
}
