#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdlib.h> /* size_t */
#include "g_vector.h"

typedef enum Tags
{
    TAG_FAIL = 0,
    TAG_REGISTER,
    TAG_LOGIN,
    TAG_CREATE_GROUP,
    TAG_JOIN_GROUP_RENDER,
    TAG_JOIN_GROUP,
    TAG_LEAVE_GROUP,
    TAG_LOGOUT,
    TAG_NUM_OF_TAGS 
} Tags;

typedef enum TagStatus
{
    TAG_ERR_SUCCESS = 0,
    TAG_ERR_NOT_INITIALIZED,
    TAG_ERR_MISMATCH,
    TAG_ERR_MESSAGE_NOT_FULL,
    TAG_ERR_NUM_OF_TAG_STATUS

} TagStatus;


typedef enum ResponseType
{
    RESPONSETYPE_REGISTRAION_FAILED = 0,
    RESPONSETYPE_REGISTRAION_SUCCESS,
    RESPONSETYPE_LOGIN_FAILED,
    RESPONSETYPE_LOGIN_SUCCES,
    RESPONSETYPE_CREATE_GROUP_FAILED,
    RESPONSETYPE_CREATE_GROUP_SUCCES,
    RESPONSETYPE_JOIN_RENDER_FAILED,
    RESPONSETYPE_JOIN_RENDER_SUCCESS,
    RESPONSETYPE_JOIN_GROUP_FAIL,
    RESPONSETYPE_JOIN_GROUP_SUCCESS,
    RESPONSETYPE_LEAVE_GROUP_FAIL,
    RESPONSETYPE_LEAVE_GROUP_SUCCESS,
    RESPONSE_LOG_OUT_FAIL,
    RESPONSE_LOG_OUT_SUCCESS,
    RESPONSETYPE_NUM_OF_RESPONSES
} ResponseType;


/**
 * @brief recieve a buffer with a protcol.
 *        return the first byte which is saved for PROTOCOL TAG TYPE.
 *
 * @param _buffer the protocl given.
 *
 * @return 0 in case _buffer is NULL.\
 *         Tag type on succuess.
 */
Tags RetrunTag(char _buffer[]);

/**
 * @brief pack user name and password into a protocl, return protcol size in bytes.
 * protocol order is:
 * TAG Type, Total Lengh, User Name Lenght, User Name Value, Password Length, Password Value
 *
 * @param _buffer the buffer to write to protcol into.
 * @param _userName user name given
 * @param _pass password given
 *
 * @return 0 in case _buffer or _userName or _pass are NULL
 * @return _buffer size in bytes.
 */
size_t PackRegistration(char *_buffer, char _userName[], char _pass[]);

/**
 * @brief unpack user name and password from protocol
 *
 * @param _buffer - the protocol given
 * @param _messageLen - the length of the message recieved
 * @param _userName - string to store the user name into.
 * @param pass - string to store the password into
 *
 * @return TAG_ERR_NOT_INITIALIZED in case _buffer or _userName or _pass are NULL.
 * @return TAG_ERR_MISMATCH in case TAG is not matching to regestriation action.
 * @return TAG_ERR_MESSAGE_NOT_FULL in case  messageLen recived is not equal to protocol total length.
 * @return TAG_ERR_OK on success
 */
TagStatus UnPackRegistration(char _buffer[], size_t _messageLen, char _userName[], char pass[]);

/**
 * @brief pack user name and password into a protocl, return protcol size in bytes.
 * protocol order is:
 * TAG Type, Total Lengh, User Name Lenght, User Name Value, Password Length, Password Value
 *
 * @param _buffer the buffer to write to protcol into.
 * @param _userName user name given
 * @param _pass password given
 *
 * @return 0 in case _buffer or _userName or _pass are NULL
 * @return _buffer size in bytes.
 */
size_t PackLogin(char _buffer[], char _userName[], char _pass[]);

/**
 * @brief unpack user name and password from protocol
 *
 * @param _buffer - the protocol given
 * @param _messageLen - the length of the message recieved
 * @param _userName - string to store the user name into.
 * @param pass - string to store the password into
 *
 * @return TAG_ERR_NOT_INITIALIZED in case _buffer or _userName or _pass are NULL.
 * @return TAG_ERR_MISMATCH in case TAG is not matching to LOGIN action.
 * @return TAG_ERR_MESSAGE_NOT_FULL in case  messageLen recived is not equal to protocol total length.
 * @return TAG_ERR_OK on success
 */
TagStatus UnPackLogin(char _buffer[], size_t _messageLen, char _userName[], char _pass[]);

/**
 * @brief pack message into a protocl, return protcol size in bytes.
 * protocol order is:
 * TAG Type, Total Lengh, Message Length, Message Value
 *
 * @param _buffer - the buffer to write to protcol into.
 * @param _msg - the given message
 *
 * @return 0 in case buffer or _msg are NULL
 * @return the _buffer size in bytes
 */
size_t PackResponseReg(char _buffer[], char _msg);

/**
 * @brief unpack message from protocol to a string
 *
 * @param _buffer - the protocol given.
 * @param _messageLen - the message length given.
 * @param _msg  - string to store the message.
 *
 * @return TAG_ERR_NOT_INITIALIZED in case _buffer or _msg are NULL.
 * @return TAG_ERR_MISMATCH in case TAG is not matching to TAG_RESPONSE action.
 * @return TAG_ERR_MESSAGE_NOT_FULL in case messageLen recived is not equal to protocol total length.
 * @return TAG_ERR_OK on success
 */
TagStatus UnPackResponseReg(char _buffer[], size_t _messageLen, char *_msg);

/**
 * @brief pack message into a protocl, return protcol size in bytes.
 * protocol order is:
 * TAG Type, Total Lengh, Message Length, Message Value
 *
 * @param _buffer - the buffer to write to protcol into.
 * @param _msg - the given message
 *
 * @return 0 in case buffer or _msg are NULL
 * @return the _buffer size in bytes
 */
size_t PackResponseLogin(char _buffer[], char _msg);

/**
 * @brief unpack message from protocol to a string
 *
 * @param _buffer - the protocol given.
 * @param _messageLen - the message length given.
 * @param _msg  - string to store the message.
 *
 * @return TAG_ERR_NOT_INITIALIZED in case _buffer or _msg are NULL.
 * @return TAG_ERR_MISMATCH in case TAG is not matching to TAG_RESPONSE action.
 * @return TAG_ERR_MESSAGE_NOT_FULL in case messageLen recived is not equal to protocol total length.
 * @return TAG_ERR_OK on success
 */
TagStatus UnPackResponseLogin(char _buffer[], size_t _messageLen, char *_msg);




/**
 * @brief pack user name and group name into a protcol, store it into buffer and return buffer bytes number.
 * protocol order is:
 * TAG Type, Total Lengh, group name length, group name Value
 * @param _buffer  - the buffer to write to protcol into.
 * @param _userName - user name provided.
 * @param _groupName - group name provided.
 * @return - number of byets in the buffer.
 */
size_t PackCreateGroup(char _buffer[], char _userName[], char _groupName[]);

/**
 * @brief unpack user name and group from protcol stored in buffer.
 *
 * @param _buffer - protocol contain user name and group name.
 * @param _messageLen  - total lenght of message.
 * @param _userName - string to store the the user name.
 * @param _groupName - string to store the the group name
 * @return TAG_ERR_SUCCESS in case good.
 */
TagStatus UnPackCreateGroup(char _buffer[], size_t _messageLen, char _userName[], char _groupName[]);

/**
 * @brief pack ip address into a protcol, store it into buffer and return buffer bytes number.
 * protocol order is:
 * TAG Type, Total Lengh, ip address length, ip address Value, port length, port value
 * @param _buffer  - the buffer to write to protcol into.
 * @param _ip - IP provided.
 * @param _port - port provided
 * @return - number of byets in the buffer.
 */
size_t PackResponseCreateGroup(char *_buffer, char _ip[], char _port[]);

/**
 * @brief unpack user name and group from protcol stored in buffer.
 *
 * @param _buffer - protocol contain user name and group name.
 * @param _messageLen  - total lenght of message.
 * @param _ip - string to store the the ip.
 * @param _port - string to store the the port.
 * @return TAG_ERR_SUCCESS in case good.
 */
TagStatus UnPackResponseCreateGroup(char _buffer[], size_t _messageLen, char _ip[], char _port[]);

/**
 * @brief pack user name and group name into a protcol, store it into buffer and return buffer bytes number.
 * protocol order is:
 * TAG Type, Total Lengh, group name length, group name Value
 * @param _buffer  - the buffer to write to protcol into.
 * @param _userName - user name provided.
 * @param _groupName - group name provided.
 * @return - number of byets in the buffer.
 */
size_t PackJoinGroup(char _buffer[], char _userName[], char _groupName[]);

/**
 * @brief unpack user name and group from protcol stored in buffer.
 *
 * @param _buffer - protocol contain user name and group name.
 * @param _messageLen  - total lenght of message.
 * @param _userName - string to store the the user name.
 * @param _groupName - string to store the the group name
 * @return TAG_ERR_SUCCESS in case good.
 */
TagStatus UnPackJoinGroup(char _buffer[], size_t _messageLen, char _userName[], char _groupName[]);

/**
 * @brief pack ip address into a protcol, store it into buffer and return buffer bytes number.
 * protocol order is:
 * TAG Type, Total Lengh, ip address length, ip address Value, port length, port value
 * @param _buffer  - the buffer to write to protcol into.
 * @param _ip - IP provided.
 * @param _port - port provided
 * @return - number of byets in the buffer.
 */
size_t PackResponseJoinGroup(char *_buffer, char _ip[], char _port[]);

/**
 * @brief unpack user name and group from protcol stored in buffer.
 *
 * @param _buffer - protocol contain user name and group name.
 * @param _messageLen  - total lenght of message.
 * @param _ip - string to store the the ip.
 * @param _port - string to store the the port.
 * @return TAG_ERR_SUCCESS in case good.
 */
TagStatus UnPackResponseJoinGroup(char _buffer[], size_t _messageLen, char _ip[], char _port[]);

/**
 * @brief store join group render TAG into a protcol.
 * protocol order is:
 * TAG Type, Total Lengh.
 * @param _bufer - buffe to store the protcol
 * @return number of byets in protcol (2)
 */
size_t PackJoinGroupRender(char *_bufer);

/**
 * @brief unpack
 *
 * @param _buffer
 * @param _messegeLen
 * @return TagStatus
 */
TagStatus UnPackJoinGroupRender(char _buffer[], size_t _messegeLen);

/**
 * @brief
 *
 * @param _buffer
 * @param _groups
 * @return size_t
 */
size_t PackRenderResponse(char _buffer[], Vector *_groups);

/**
 * @brief
 *
 * @param _buffer
 * @param _messageLen
 * @param _groups
 * @return TagStatus
 */
TagStatus UnPackRenderResponse(char _buffer[], size_t _messageLen, Vector *_groups);

/**
 * @brief
 *
 * @param _buffer
 * @param _groups
 * @return size_t
 */
size_t RetrunNumOfGroups(char _buffer[]);

/**
 * @brief 
 * 
 * @param _buffer 
 * @return size_t 
 */
size_t PackRender(char _buffer[]);

/**
 * @brief 
 * 
 * @param _buffer 
 * @param _userName 
 * @param _groupName 
 * @return size_t 
 */
size_t PackJoinGroup(char _buffer[], char _userName[], char _groupName[]);

/**
 * @brief 
 * 
 * @param _buffer 
 * @param _messageLen 
 * @param _userName 
 * @param _groupName 
 * @return TagStatus 
 */
TagStatus UnPackJoinGroup(char _buffer[], size_t _messageLen, char _userName[], char _groupName[]);

/**
 * @brief 
 * 
 * @param _buffer 
 * @param _ip 
 * @param _port 
 * @return size_t 
 */
size_t PackResponseJoinGroup(char* _buffer, char _ip[], char _port[]);

/**
 * @brief 
 * 
 * @param _buffer 
 * @param _messageLen 
 * @param _ip 
 * @param _port 
 * @return TagStatus 
 */
TagStatus UnPackResponseJoinGroup(char _buffer[], size_t _messageLen, char _ip[], char _port[]);

/**
 * @brief 
 * 
 * @param _buffer 
 * @param _userName 
 * @param _groupName 
 * @return size_t 
 */
size_t PackLeaveGroup(char _buffer[], char _userName[], char _groupName[]);

/**
 * @brief 
 * 
 * @param _buffer 
 * @param _messageLen 
 * @param _userName 
 * @param _groupName 
 * @return TagStatus 
 */
TagStatus UnPackLeaveGroup(char _buffer[], size_t _messageLen, char _userName[], char _groupName[]);

/**
 * @brief 
 * 
 * @param _buffer 
 * @param _msg 
 * @return size_t 
 */
size_t PackResponseLeaveGroup(char _buffer[], char _msg);

/**
 * @brief 
 * 
 * @param _buffer 
 * @param _messageLen 
 * @param _msg 
 * @return TagStatus 
 */
TagStatus UnPackResponseLeaveGroup(char _buffer[], size_t _messageLen, char *_msg);

/**
 * @brief 
 * 
 * @param _buffer 
 * @param _msg 
 * @return size_t 
 */
size_t PackResponseLeaveGroup(char _buffer[], char _msg);



/**
 * @brief 
 * 
 * @param _buffer 
 * @param _userName 
 * @return size_t 
 */
size_t PackLogout(char _buffer[], char _userName[]);


/**
 * @brief 
 * 
 */
TagStatus UnPackLogout(char _buffer[], size_t _messageLen, char _userName[]);



/**
 * @brief 
 * 
 */
size_t PackResponseLogout(char _buffer[], char _msg);


/**
 * @brief 
 * 
 */
TagStatus UnPackResponseLogout(char _buffer[], size_t _messageLen, char *_msg);


#endif /* __PROTOCOL_H__  */